#include "ModelImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>

#include "Logger.h"
#include <filesystem>
#include "MeshBuilder.h"
#include "Resource.h"
#include "Material.h"
#include "Entity.h"
#include "Component.h"
#include "Shader.h"
#include "Engine.h"
#include "CacheSystem.h"
#include "Scene.h"
#include "Assets.h"
#include "AssimpGLMHelpers.h"
#include "Factory.h"
#include "MeshExporter.h"

ModelImporter::ModelImporter()
{
	Engine::get()->registerSubSystem<ModelImporter>(this);

	m_importer = std::make_shared<Assimp::Importer>();

	logInfo("Model importer init successfully.");
}

ModelImporter::ModelInfo ModelImporter::import(const std::string& path)
{
	ModelImporter::ModelInfo mInfo;

	mInfo.mesh = Factory<Mesh>::create();

	if (!std::filesystem::exists(path))
	{
		logError("File doesn't exists: " + path);
		return {};
	}

	auto fileDir = std::filesystem::path(path).parent_path().string();

	const aiScene* scene = nullptr;

	// read scene from file
	scene = m_importer->ReadFile(path, 
		aiProcess_Triangulate | 
		aiProcess_GenSmoothNormals | 
		aiProcess_FlipUVs | 
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logError("ERROR::ASSIMP::{}", m_importer->GetErrorString());
		return {};
	}

	auto savedFilePath = MeshExporter::exportMesh(mInfo.mesh, scene);

	if (scene->HasMaterials())
	{
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			auto& aMaterial = scene->mMaterials[i];

			auto& diffuse = importAiMaterialTexture(aMaterial, aiTextureType::aiTextureType_DIFFUSE, fileDir);
			if (!diffuse.isEmpty())
			{
				mInfo.textures.push_back(diffuse);
			}

			auto& normal = importAiMaterialTexture(aMaterial, aiTextureType::aiTextureType_NORMALS, fileDir);
			if (!normal.isEmpty())
			{
				mInfo.textures.push_back(normal);
			}
		}
	}

	load(savedFilePath, mInfo);

	return mInfo;
}

ModelImporter::ModelInfo ModelImporter::load(const std::string & path, ModelImporter::ModelInfo& modelInfo)
{
	if (!std::filesystem::exists(path))
	{
		logError("File doesn't exists: " + path);
		return {};
	}

	const aiScene* scene = nullptr;

	// If the scene was previously loaded last, we can optimize the load since it is already in memory.
	if (path == m_lastLoadedSceneName)
	{
		scene = m_importer->GetScene();
	}
	else
	{

		// read scene from file
		scene = m_importer->ReadFile(path, aiProcess_ValidateDataStructure);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			logError("ERROR::ASSIMP::{}", m_importer->GetErrorString());
			return {};
		}
	}

	m_lastLoadedSceneName = path;

	std::string modelName = std::filesystem::path(path).filename().string();
	modelName = modelName.substr(0, modelName.find_first_of('.'));

	// create new model session
	ModelImportSession session;
	session.filepath = path;
	session.fileDir = std::filesystem::path(path).parent_path().string();
	session.name = modelName;
	session.builder = &MeshBuilder::builder();

	// extract mesh from root node
	processNode(scene->mRootNode, scene, session);

	// build mesh
	session.builder->build(modelInfo.mesh);

	if (scene->HasMaterials())
	{
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			auto& material = std::make_shared<Material>();
			auto& aMaterial = scene->mMaterials[i];

			// get uuid using tex name from association map


			// load texture

			aiString diffuseStr;
			if (aMaterial->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &diffuseStr) == aiReturn_SUCCESS)
			{
				UUID uuid = Engine::get()->getMemoryManagementSystem()->getAssociation(diffuseStr.C_Str());
				Resource<Texture> texture = Resource<Texture>(uuid);
				material->setTexture(Texture::Type::Albedo, texture);
			}

			aiString normalStr;
			if (aMaterial->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &normalStr) == aiReturn_SUCCESS)
			{
				UUID uuid = Engine::get()->getMemoryManagementSystem()->getAssociation(normalStr.C_Str());
				Resource<Texture> texture = Resource<Texture>(uuid);
				material->setTexture(Texture::Type::Normal, texture);
			}

			if (material->getAllTextures().size() > 0)
			{
				modelInfo.materials.push_back(material);
			}
		}
	}

	

	return modelInfo;
}

void ModelImporter::processNode(aiNode* node, const aiScene* scene, ModelImporter::ModelImportSession& session)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		processMesh(scene->mMeshes[node->mMeshes[i]], scene, session);
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, session);
	}
}

struct BoneWeight
{
	unsigned int boneID = -1;
	float weight = 0.f;
};

void ModelImporter::processMesh(aiMesh* mesh, const aiScene* scene, ModelImporter::ModelImportSession& session)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> texcoords;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// process vertex positions, normals and texture coordinates
		if (mesh->HasPositions())
		{
			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			positions.emplace_back(pos);
		}

		if (mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			normals.emplace_back(normal);
		}

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			texcoords.emplace_back(vec);
		}
		else
		{
			texcoords.emplace_back(glm::vec2(0.0f, 0.0f));
		}

		if (mesh->HasTangentsAndBitangents())
		{
			tangents.emplace_back(glm::vec3{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z });
		}
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	
	
	if (mesh->HasBones())
	{
		std::vector<glm::ivec3> bonesIDs;
		std::vector<glm::vec3> bonesWeights;

		// Will be used by mesh as base array for bone transformations uniform
		std::vector<glm::mat4> bonesOffsets;

		// an intermediate helper map used to facilitate in extracting bone weight for each vertex
		std::map<int, std::map<float, BoneWeight>> vertexToBoneMap;

		// Extract Bone to ID map 
		auto& boneNameToIDMap = session.boneNameToIDMap;
		auto& boneCount = session.boneCount;

		// Iterate all bones in Assimp model
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			auto bone = mesh->mBones[i];
			auto boneName = bone->mName.C_Str();

			// a new bone is found, increment bone ID and add bone offset to offsets array
			if (boneNameToIDMap.find(boneName) == boneNameToIDMap.end())
			{
				boneNameToIDMap[boneName] = boneCount++;
				bonesOffsets.push_back(AssimpGLMHelpers::convertMat4ToGLMFormat(bone->mOffsetMatrix));
			}

			// Extract weights for each vertex, we use a map container and a (-weight) key so the entries will be sorted in descending order when we iterate it
			int numOfWeights = bone->mNumWeights;
			auto weights = bone->mWeights;
			for (int j = 0; j < numOfWeights; j++)
			{
				unsigned int vertexID = weights[j].mVertexId;
				float weight = weights[j].mWeight;
				vertexToBoneMap[vertexID][-weight] = BoneWeight{ boneNameToIDMap[boneName] , weight };
			}
		}
		
		bonesIDs.resize(vertexToBoneMap.size(), glm::ivec3(- 1));
		bonesWeights.resize(vertexToBoneMap.size());

		// post process influence data in bone info map, we discard the least influential bone weights
		for (auto& [vID, boneInfluenceMap] : vertexToBoneMap)
		{
			// TODO resize and normalize
			//if (boneInfluenceMap.size() > 3)
			//{
			//	boneInfluenceMap.resize(3);
			//}
			auto boneIter = boneInfluenceMap.begin();
			int index = 0;
			while (boneIter != boneInfluenceMap.end() && index < 3)
			{
				bonesIDs[vID][index] = (int)(*boneIter).second.boneID;
				bonesWeights[vID][index] = (*boneIter).second.weight;
				boneIter++;
				index++;
			}
		}

		(*session.builder)
			.addBoneIDs(bonesIDs)
			.addBoneWeights(bonesWeights)
			.addBonesInfo(bonesOffsets, boneNameToIDMap);
	}

	(*session.builder)
		.addPositions(positions)
		.addNormals(normals)
		.addTexcoords(texcoords)
		.addIndices(indices)
		.addTangents(tangents);
}

Resource<Texture> ModelImporter::importAiMaterialTexture(aiMaterial* mat, aiTextureType type, const std::string& dir)
{
	aiString str;
	if (mat->GetTexture(type, 0, &str) != aiReturn_SUCCESS)
	{
		return Resource<Texture>::empty;
	}
	auto texture = Engine::get()->getSubSystem<Assets>()->importTexture2D(dir + "/" + str.C_Str(), true);
	return texture;
}

Texture::Type ModelImporter::getTextureType(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType::aiTextureType_DIFFUSE:
		return Texture::Type::Diffuse;
	case aiTextureType::aiTextureType_SPECULAR:
		return Texture::Type::Specular;
	case aiTextureType::aiTextureType_HEIGHT:
		return Texture::Type::Normal;
	default:
		logError("Unsupported type: " + type);
		return Texture::Type::None;
	}
}
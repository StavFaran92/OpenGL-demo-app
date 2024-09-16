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

	init();
}

void ModelImporter::init()
{
	m_importer = std::make_shared<Assimp::Importer>();

	logInfo("Model importer init successfully.");
}

Resource<Mesh> ModelImporter::import(const std::string& path)
{
	Resource<Mesh> mesh = Factory<Mesh>::create();

	// validate init
	if (m_importer == nullptr)
	{
		logError("Importer not initialized.");
		return Resource<Mesh>::empty;
	}

	if (!std::filesystem::exists(path))
	{
		logError("File doesn't exists: " + path);
		return Resource<Mesh>::empty;
	}

	const aiScene* scene = nullptr;

	// read scene from file
	scene = m_importer->ReadFile(path, 
		//aiProcess_Triangulate | 
		aiProcess_GenSmoothNormals | 
		aiProcess_FlipUVs | 
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logError("ERROR::ASSIMP::{}", m_importer->GetErrorString());
		return Resource<Mesh>::empty;
	}

	auto savedFilePath = MeshExporter::exportMesh(mesh, scene);

	load(savedFilePath, mesh);

	return mesh;
}

Resource<Mesh> ModelImporter::load(const std::string & path, Resource<Mesh> mesh)
{
	// validate init
	if (m_importer == nullptr)
	{
		logError("Importer not initialized.");
		return Resource<Mesh>::empty;
	}

	if (!std::filesystem::exists(path))
	{
		logError("File doesn't exists: " + path);
		return Resource<Mesh>::empty;
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
			return Resource<Mesh>::empty;
		}
	}

	m_lastLoadedSceneName = path;

	std::string modelName = path.substr(path.find_last_of('/\\') + 1);
	modelName = modelName.substr(0, modelName.find_first_of('.'));

	// create new model session
	ModelImportSession session;
	session.filepath = path;
	session.fileDir = path.substr(0, path.find_last_of('/\\'));
	session.name = modelName;
	session.builder = &MeshBuilder::builder();

	processNode(scene->mRootNode, scene, session);

	session.builder->build(mesh);

	return mesh;
}

void ModelImporter::processNode(aiNode* node, const aiScene* scene, ModelImporter::ModelImportSession& session)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		session.nodeIndex = i;
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		std::string meshName = session.filepath + "_" + std::to_string(session.nodeIndex) + "_" + std::to_string(session.childIndex);
		auto memoryManager = Engine::get()->getMemoryManagementSystem();
		processMesh(aimesh, scene, session);
		//Resource<Mesh> mesh = memoryManager->createOrGetCached<Mesh>(meshName, [&]() {  });
		//entity.getComponent<MeshComponent>().mesh = mesh;

		//TODO fix the code below

		//auto textureHandlers = new std::vector<Resource<Texture>>();

		//auto& sgeMaterial = entity.addComponent<Material>();
		//// process material
		//aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];

		//auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, session);
		//textureHandlers->insert(textureHandlers->end(), diffuseMaps.begin(), diffuseMaps.end());

		//if (diffuseMaps.size() > 0)
		//{
		//	sgeMaterial.setTexture(Texture::Type::Diffuse, Resource<Texture>(diffuseMaps[0]));
		//}

		//auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, session);
		//textureHandlers->insert(textureHandlers->end(), specularMaps.begin(), specularMaps.end());

		////TODO fix
		//if (specularMaps.size() > 0)
		//{
		//	sgeMaterial.setTexture(Texture::Type::Specular, Resource<Texture>(specularMaps[0]));
		//}

		//auto normalMap = loadMaterialTextures(material, aiTextureType_HEIGHT, session);
		//if (normalMap.size() > 0)
		//{
		//	sgeMaterial.setTexture(Texture::Type::Normal, Resource<Texture>(normalMap[0]));
		//}
		
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		session.childIndex = i;
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

std::vector<Resource<Texture>> ModelImporter::loadMaterialTextures(aiMaterial* mat, aiTextureType type, ModelImporter::ModelImportSession& session)
{
	std::vector<Resource<Texture>> textureHandlers;

	// Iterate material's textures
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		auto textureName = str.C_Str();

		// Texture not found in cache -> load it and add to cache
		auto textureHandler = Engine::get()->getSubSystem<Assets>()->importTexture2D(session.fileDir + "/" + textureName);
		textureHandlers.push_back(textureHandler);
	}
	return textureHandlers;
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
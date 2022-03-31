#include "ModelImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Logger.h"
#include <filesystem>

ModelImporter::ModelImporter()
{
	init();
}

void ModelImporter::init()
{
	m_importer = std::make_shared<Assimp::Importer>();

	logInfo("Model importer init successfully.");
}

Model* ModelImporter::loadModelFromFile(const std::string& path, bool flipTextures /*= false*/)
{
	// validate init
	if (m_importer == nullptr)
	{
		logError("Importer not initialized.");
		return nullptr;
	}

	if (!std::filesystem::exists(path))
	{
		logError("File doesn't exists: " + path);
		return nullptr;
	}

	// read scene from file
	const aiScene* scene = m_importer->ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logError("ERROR::ASSIMP::{}", m_importer->GetErrorString());
		return nullptr;
	}

	//create new model
	auto model = new Model();

	// create new model session
	ModelImportSession session;
	session.flippedTexture = flipTextures;
	session.filepath = path;
	session.fileDir = path.substr(0, path.find_last_of('\\'));

	// place session in session map
	m_sessions[model->getID()] = session;

	processNode(scene->mRootNode, scene, session, *model);

	return model;
}

void ModelImporter::processNode(aiNode* node, const aiScene* scene, ModelImporter::ModelImportSession& session, Model& model)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model.addMesh(processMesh(mesh, scene, session));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, session, model);
	}
}

Mesh* ModelImporter::processMesh(aiMesh* mesh, const aiScene* scene, ModelImporter::ModelImportSession& session)
{
	auto positions = std::make_shared<std::vector<glm::vec3>>();
	auto normals = std::make_shared<std::vector<glm::vec3>>();
	auto texcoords = std::make_shared<std::vector<glm::vec2>>();
	auto indices = std::make_shared<std::vector<unsigned int>>();
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// process vertex positions, normals and texture coordinates
		if (mesh->HasPositions())
		{
			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			positions->emplace_back(pos);
		}

		if (mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			normals->emplace_back(normal);
		}

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			texcoords->emplace_back(vec);
		}
		else
		{
			texcoords->emplace_back(glm::vec2(0.0f, 0.0f));
		}
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices->push_back(face.mIndices[j]);
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, session);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, session);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	Mesh* generatedMesh = new Mesh();
	generatedMesh->setNumOfVertices(mesh->mNumVertices);
	generatedMesh->setPositions(positions);
	generatedMesh->setNormals(normals);
	generatedMesh->setTexcoords(texcoords);
	generatedMesh->addTextures(textures);
	generatedMesh->setIndices(indices);
	generatedMesh->build();

	return generatedMesh;
}

std::vector<std::shared_ptr<Texture>> ModelImporter::loadMaterialTextures(aiMaterial* mat, aiTextureType type, ModelImporter::ModelImportSession& session)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesCache.size(); j++)
		{
			if (m_texturesCache[j]->getFilepath().compare(session.fileDir + "\\" + str.C_Str()) == 0)
			{
				textures.push_back(m_texturesCache[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			auto texture = Texture::loadTextureFromFile(session.fileDir + "\\" + str.C_Str());
			auto pType = getTextureType(type);
			if (pType != Texture::Type::None)
			{
				texture->setType(pType);
				textures.push_back(texture);
				m_texturesCache.push_back(texture);
			}
		}
	}
	return textures;
}

Texture::Type ModelImporter::getTextureType(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType::aiTextureType_DIFFUSE:
		return Texture::Type::Diffuse;
	case aiTextureType::aiTextureType_SPECULAR:
		return Texture::Type::Specular;
	default:
		logError("Unsupported type: " + type);
		return Texture::Type::None;
	}
}
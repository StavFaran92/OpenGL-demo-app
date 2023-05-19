#include "ModelImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Logger.h"
#include <filesystem>
#include "MeshBuilder.h"
#include "TextureHandler.h"
#include "ObjectFactory.h"
#include "Model.h"
#include "ObjectHandler.h"

ModelImporter::ModelImporter()
{
	init();
}

void ModelImporter::init()
{
	m_importer = std::make_shared<Assimp::Importer>();

	logInfo("Model importer init successfully.");
}

ObjectHandler<Model> ModelImporter::loadModelFromFile(const std::string& path)
{
	// validate init
	if (m_importer == nullptr)
	{
		logError("Importer not initialized.");
		return ObjectHandler<Model>::EmptyHandler;
	}

	if (!std::filesystem::exists(path))
	{
		logError("File doesn't exists: " + path);
		return ObjectHandler<Model>::EmptyHandler;
	}

	// read scene from file
	const aiScene* scene = m_importer->ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logError("ERROR::ASSIMP::{}", m_importer->GetErrorString());
		return ObjectHandler<Model>::EmptyHandler;
	}

	//create new model
	auto modelHandler = ObjectFactory::create<Model>();

	std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
	modelHandler.object()->setMaterial(material);

	// create new model session
	ModelImportSession session;
	session.filepath = path;
	session.fileDir = path.substr(0, path.find_last_of('\\'));

	// place session in session map
	m_sessions[modelHandler.getID()] = session;

	processNode(scene->mRootNode, scene, session, *modelHandler.object());

	return modelHandler;
}

void ModelImporter::processNode(aiNode* node, const aiScene* scene, ModelImporter::ModelImportSession& session, Model& model)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model.addMesh(processMesh(mesh, scene, session));
		auto textureHandlers = new std::vector<TextureHandler*>();

		auto material = model.getMaterial();
		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, session);
			textureHandlers->insert(textureHandlers->end(), diffuseMaps.begin(), diffuseMaps.end());

			auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, session);
			textureHandlers->insert(textureHandlers->end(), specularMaps.begin(), specularMaps.end());
		}
		material->addTextureHandlers(*textureHandlers);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, session, model);
	}
}

Mesh* ModelImporter::processMesh(aiMesh* mesh, const aiScene* scene, ModelImporter::ModelImportSession& session)
{
	auto positions = new std::vector<glm::vec3>();
	auto normals = new std::vector<glm::vec3>();
	auto texcoords = new std::vector<glm::vec2>();
	auto indices = new std::vector<unsigned int>();

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

	return MeshBuilder::builder()
		.setPositions(*positions)
		.setNormals(*normals)
		.setTexcoords(*texcoords)
		.setIndices(*indices)
		.build();
}

std::vector<TextureHandler*> ModelImporter::loadMaterialTextures(aiMaterial* mat, aiTextureType type, ModelImporter::ModelImportSession& session)
{
	std::vector<TextureHandler*> textureHandlers;

	// Iterate material's textures
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		auto textureName = str.C_Str();

		// Texture not found in cache -> load it and add to cache
		auto textureHandler = Texture::loadTextureFromFile(session.fileDir + "\\" + textureName);
		auto pType = getTextureType(type);
		if (pType != Texture::Type::None)
		{
			textureHandler->setType(pType);
			textureHandlers.push_back(textureHandler);
		}
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
	default:
		logError("Unsupported type: " + type);
		return Texture::Type::None;
	}
}
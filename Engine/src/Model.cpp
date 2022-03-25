#include "Model.h"

#include "ApplicationConstants.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PointLight.h"

#include "Resources/Primitives/quad.h"
#include "Resources/Primitives/cube.h"

#include "Context.h"
#include "Scene.h"
#include "Skybox.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Vertex.h"
#include "IRenderer.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Engine.h"

Model::Model()
{
	logTrace(__FUNCTION__);

	m_transformation = std::make_shared<Transform>();

	m_shader = std::shared_ptr<Shader>(Shader::PhongShader());
}

Model::~Model()
{
	logTrace(__FUNCTION__);
}

Model* Model::loadModelFromFile(const std::string& path, bool flipTexture /*= false*/)
{
	auto model = new Model();

	model->FlipTexture(flipTexture);

	model->m_path = path;
	model->m_modelDir = model->m_path.substr(0, model->m_path.find_last_of('\\'));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model->m_path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logError("ERROR::ASSIMP::{}", importer.GetErrorString());
		return nullptr;
	}

	model->processNode(scene->mRootNode, scene);

	return model;
}

Model* Model::createPrimitiveModel(PrimitiveType ptype)
{
	auto model = new Model();

	std::shared_ptr<Mesh> mesh = nullptr;

	if (ptype == PrimitiveType::Quad)
	{
		mesh = std::make_shared<Mesh>((float*)Primtives::Quad::vertices, sizeof(Primtives::Quad::vertices),
			(unsigned int*)Primtives::Quad::indices, sizeof(Primtives::Quad::indices));
		//mesh = std::make_shared<Mesh>();
		//Mesh::VerticesLayout layout;
		//layout.numOfVertices = 4;
		//layout.entries.emplace("positions", 3);
		//layout.entries.emplace("normals", 3);
		//layout.entries.emplace("texcoords", 2);
		//mesh->setRawVertices((float*)Primtives::Quad::vertices, layout);
		//auto indices = std::make_shared<std::vector<unsigned int>>();
		//for (unsigned int i : Primtives::Quad::indices)
		//{
		//	indices->push_back(i);
		//}
		//mesh->setIndices(indices);
		//mesh->build();
	}
	else if (ptype == PrimitiveType::Cube)
	{
		mesh = std::make_shared<Mesh>((float*)Primtives::Cube::vertices, sizeof(Primtives::Cube::vertices));
	}

	auto texturediff = Texture::LoadTextureFromFile("Resources\\Textures\\template.png");
	texturediff->SetType(Constants::g_textureDiffuse);

	auto textureSpec = Texture::LoadTextureFromFile("Resources\\Textures\\template.png");
	textureSpec->SetType(Constants::g_textureSpecular);

	mesh->addTexture(texturediff);
	mesh->addTexture(textureSpec);

	std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
	model->UseMaterial(material);

	model->m_meshes.push_back(mesh);

	return model;
}


void Model::Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader /* = nullptr*/)
{
	auto currShader = m_shader;

	if (shader)
		currShader = shader;

	auto context = Engine::get()->getContext();
	if (context->getActiveScene()->getSkybox())
	{
		if (m_isReflective)
		{
			currShader = context->GetReflectionShader();
			currShader->UseShader();
			currShader->SetInt("skybox", 0);
			auto textures = context->getActiveScene()->getSkybox()->GetTextures();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->Bind();
		}

		if (m_isRefractive)
		{
			currShader = context->GetRefractiveShader();
			currShader->UseShader();
			currShader->SetInt("skybox", 0);
			currShader->SetFloat("refractiveRatio", 1 / 1.52f);
			auto textures = context->getActiveScene()->getSkybox()->GetTextures();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->Bind();
		}
	}

	currShader->SetMat4("model", m_transformation->GetTransformation());

	if (currShader->IsMaterialsEnabled() && m_material)
	{
		m_material->UseMaterial(currShader);
	}

	renderer->SetDrawType(Renderer::DrawType::Triangles);

	for (auto i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->renderMesh(currShader, renderer);
	}
}

bool Model::AttachShader(std::shared_ptr<Shader> shader)
{
	m_shader = shader;

	return true;
}

bool Model::DetachShader()
{
	m_shader = Engine::get()->getRenderer()->GetDefaultShader();

	return true;
}

void Model::update(float delta)
{
	m_transformation->update(delta);
}

bool Model::UseShader()
{
	m_shader->UseShader();

	return true;
}





std::vector<std::shared_ptr<Texture>> Model::GetTextures()
{
	std::vector<std::shared_ptr<Texture>> result;
	
	for (const auto &mesh : m_meshes)
	{
		auto textures = mesh->getTextures();
		result.insert(result.end(), textures.begin(), textures.end());
	}

	return result;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<Shader> Model::GetShader()
{
	return m_shader;
}

bool Model::UseMaterial(std::shared_ptr<Material> material)
{
	m_material = material;

	return true;
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	//auto vertices = std::make_shared<std::vector<Vertex>>();
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

		auto diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, Constants::g_textureDiffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		auto specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, Constants::g_textureSpecular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	std::shared_ptr<Mesh> generatedMesh = std::make_shared<Mesh>();
	generatedMesh->setNumOfVertices(mesh->mNumVertices);
	generatedMesh->setPositions(positions);
	generatedMesh->setNormals(normals);
	generatedMesh->setTexcoords(texcoords);
	generatedMesh->addTextures(textures);
	generatedMesh->setIndices(indices);
	generatedMesh->build();

	return generatedMesh;
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesCache.size(); j++)
		{
			if (m_texturesCache[j]->GetPath().compare(m_modelDir + "\\" + str.C_Str()) == 0)
			{
				textures.push_back(m_texturesCache[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			auto texture = Texture::LoadTextureFromFile(m_modelDir + "\\" + str.C_Str(), m_flipTexture);
			texture->SetType(typeName);
			textures.push_back(texture);
			m_texturesCache.push_back(texture);
		}
	}
	return textures;
}

void Model::FlipTexture(bool flip)
{
	m_flipTexture = flip;
}

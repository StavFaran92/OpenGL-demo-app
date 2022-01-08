#include "Model.h"

#include "Renderer/Lighting/PointLight.h"

Model::Model(const std::string& path) : m_path(path)
{
	logTrace(__FUNCTION__);
	m_modelDir = m_path.substr(0, m_path.find_last_of('\\'));

	transformation = std::make_shared<Transform>();

	m_shader = Application::Get().GetRenderer()->GetDefaultShader();
}

void Model::Draw(std::shared_ptr<Renderer> renderer)
{
	m_shader->SetMat4("model", transformation->GetTransformation());

	if (m_material)
	{
		m_material->UseMaterial(m_shader);
	}

	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->RenderMesh(m_shader, renderer);
	}
}

bool Model::AttachShader(std::shared_ptr<Shader> shader)
{
	m_shader = shader;

	return true;
}

bool Model::DetachShader()
{
	m_shader = Application::Get().GetRenderer()->GetDefaultShader();

	return true;
}

void Model::Update(float delta)
{
	transformation->Update(delta);
}

bool Model::UseShader()
{
	m_shader->UseShader();

	return true;
}



void Model::loadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logError( "ERROR::ASSIMP::{}" , importer.GetErrorString() );
		return;
	}

	processNode(scene->mRootNode, scene);
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

bool Model::UseLight(std::shared_ptr<Light> light)
{
	m_light = light;

	return true;
}

bool Model::UseMaterial(std::shared_ptr<Material> material)
{
	m_material = material;

	return true;
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	auto vertices = std::make_shared<std::vector<Vertex>>();
	auto indices = std::make_shared<std::vector<unsigned int>>();
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		if (mesh->HasPositions())
		{
			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertex.Position = pos;
		}

		if (mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.Normal = normal;
		}

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices->push_back(vertex);
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

	return std::make_shared<Mesh>(vertices, indices, textures);
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
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(m_modelDir + "\\" + str.C_Str(), typeName);
			texture->LoadTexture(m_flipTexture);
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

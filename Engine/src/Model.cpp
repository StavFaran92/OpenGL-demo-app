#include "Model.h"

#include "PointLight.h"

#include "Resources/Primitives/quad.h"
#include "Resources/Primitives/cube.h"

#include "Context.h"

Model::Model()
{
	logTrace(__FUNCTION__);

	m_transformation = std::make_shared<Transform>();

	m_shader = Engine::Get()->GetRenderer()->GetDefaultShader();
}

std::shared_ptr<Model> Model::LoadModelFromFile(const std::string& path, bool flipTexture /*= false*/)
{
	auto model = std::make_shared<Model>();

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

std::shared_ptr<Model> Model::CreatePrimitiveModel(PrimitiveType ptype)
{
	auto model = std::make_shared<Model>();

	model->SetRefraction(true);

	std::shared_ptr<Mesh> mesh = nullptr;

	switch (ptype)
	{
	case PrimitiveType::Quad:
		mesh = std::make_shared<Mesh>((float*)Primtives::Quad::vertices, sizeof(Primtives::Quad::vertices),
			(unsigned int*)Primtives::Quad::indices, sizeof(Primtives::Quad::indices));
		break;
	case PrimitiveType::Cube:
		mesh = std::make_shared<Mesh>((float*)Primtives::Cube::vertices, sizeof(Primtives::Cube::vertices));
		break;
	}

	auto texturediff = Texture::LoadTextureFromFile("Resources\\Textures\\template.png");
	texturediff->SetType(Constants::g_textureDiffuse);

	auto textureSpec = Texture::LoadTextureFromFile("Resources\\Textures\\template.png");
	textureSpec->SetType(Constants::g_textureSpecular);

	mesh->AddTexture(texturediff);
	mesh->AddTexture(textureSpec);

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

	if (m_isReflective)
	{
		auto context = Engine::Get()->GetContext();
		currShader = context->GetReflectionShader();
		currShader->UseShader();
		currShader->SetInt("skybox", 0);
		auto textures = context->GetSkyBox()->GetTextures();
		if (textures.size() <= 0)
		{
			logError("Skybox does not contain cubemap texture.");
			return;
		}
		textures[0]->Bind();
	}

	if (m_isRefractive)
	{
		auto context = Engine::Get()->GetContext();
		currShader = context->GetRefractiveShader();
		currShader->UseShader();
		currShader->SetInt("skybox", 0);
		currShader->SetFloat("refractiveRatio", 1 / 1.52f);
		auto textures = context->GetSkyBox()->GetTextures();
		if (textures.size() <= 0)
		{
			logError("Skybox does not contain cubemap texture.");
			return;
		}
		textures[0]->Bind();
	}

	currShader->SetMat4("model", m_transformation->GetTransformation());

	if (currShader->IsMaterialsEnabled() && m_material)
	{
		m_material->UseMaterial(currShader);
	}

	renderer->SetDrawType(Renderer::DrawType::Triangles);

	for (auto i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->RenderMesh(currShader, renderer);
	}
}

bool Model::AttachShader(std::shared_ptr<Shader> shader)
{
	m_shader = shader;

	return true;
}

bool Model::DetachShader()
{
	m_shader = Engine::Get()->GetRenderer()->GetDefaultShader();

	return true;
}

void Model::Update(float delta)
{
	m_transformation->Update(delta);
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
		auto textures = mesh->GetTextures();
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

	std::shared_ptr<Mesh> generatedMesh = std::make_shared<Mesh>(vertices, indices);
	generatedMesh->AddTextures(textures);

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

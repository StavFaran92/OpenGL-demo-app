#include "Model.h"

#include "ApplicationConstants.h"

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
#include "Sphere.h"
#include "ModelBuilder.h"
#include "Logger.h"
#include "MeshBuilder.h"

//template ModelBuilder<Model>* Model::builder<Model>();

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
			currShader->use();
			currShader->SetInt("skybox", 0);
			auto textures = context->getActiveScene()->getSkybox()->GetTextures();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->bind();
		}

		if (m_isRefractive)
		{
			currShader = context->GetRefractiveShader();
			currShader->use();
			currShader->SetInt("skybox", 0);
			currShader->SetFloat("refractiveRatio", 1 / 1.52f);
			auto textures = context->getActiveScene()->getSkybox()->GetTextures();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->bind();
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

void Model::addMesh(Mesh* mesh)
{
	m_meshes.push_back(std::shared_ptr<Mesh>(mesh));
}

bool Model::use()
{
	m_shader->use();

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

std::shared_ptr<Shader> Model::GetShader()
{
	return m_shader;
}

bool Model::UseMaterial(std::shared_ptr<Material> material)
{
	m_material = material;

	return true;
}

MeshBuilder* Model::createMeshBuilder()
{
	return new MeshBuilder();
}

//template<typename T>
//ModelBuilder<T>* Model::builder()
//{
//	return ModelBuilder<T>();
//}


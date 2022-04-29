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
#include "PhongShader.h"


Model::Model()
	: Object3D()
{
	logTrace(__FUNCTION__);
}

Model::~Model()
{
	logTrace(__FUNCTION__);
}

void Model::draw(IRenderer& renderer, Shader* shader /* = nullptr*/)
{
	Shader* shaderToUse = nullptr;

	if (shader)
	{
		shaderToUse = shader;
	}
	else if (m_shader)
	{
		shaderToUse = m_shader.get();
	}
	else
	{
		shaderToUse = Engine::get()->getContext()->getPhongShader();
	}

	assert(shaderToUse);

	shaderToUse->use();

	auto context = Engine::get()->getContext();
	if (context->getActiveScene()->getSkybox())
	{
		if (m_isReflective)
		{
			shaderToUse = context->GetReflectionShader();
			shaderToUse->use();
			shaderToUse->setInt("skybox", 0);
			auto textures = context->getActiveScene()->getSkybox()->getTextureHandlers();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->bind();
		}

		if (m_isRefractive)
		{
			shaderToUse = context->GetRefractiveShader();
			shaderToUse->use();
			shaderToUse->setInt("skybox", 0);
			shaderToUse->setFloat("refractiveRatio", 1 / 1.52f);
			auto textures = context->getActiveScene()->getSkybox()->getTextureHandlers();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->bind();
		}
	}

	shaderToUse->setMat4("model", m_transformation->getMatrix());

	if (shaderToUse->IsMaterialsEnabled() && m_material)
	{
		m_material->UseMaterial(*shaderToUse);
	}

	renderer.SetDrawType(Renderer::DrawType::Triangles);

	for (auto i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->render(*shaderToUse, renderer);
	}

	shaderToUse->release();
}

bool Model::attachShader(std::shared_ptr<Shader> shader)
{
	m_shader = shader;

	return true;
}

bool Model::attachShader(Shader* shader)
{
	m_shader = std::shared_ptr<Shader>(shader);

	return true;
}

bool Model::detachShader()
{
	m_shader = nullptr;

	return true;
}

void Model::addMesh(Mesh* mesh)
{
	m_meshes.push_back(std::shared_ptr<Mesh>(mesh));
}

std::vector<const TextureHandler*> Model::getTextureHandlers()
{
	std::vector<const TextureHandler*> handlers;
	
	for (const auto &mesh : m_meshes)
	{
		auto textures = mesh->getTextureHandlers();
		handlers.insert(handlers.end(), textures.begin(), textures.end());
	}

	return handlers;
}

Shader* Model::getShader()
{
	return m_shader.get();
}

bool Model::useMaterial(std::shared_ptr<Material> material)
{
	m_material = material;

	return true;
}

void Model::setReflection(bool enable)
{
	m_isReflective = enable;
}

void Model::setRefraction(bool enable)
{
	m_isRefractive = enable;
}

MeshBuilder* Model::createMeshBuilder()
{
	return &MeshBuilder::builder();
}
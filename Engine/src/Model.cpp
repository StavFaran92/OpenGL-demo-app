#include "Model.h"

#include "ApplicationConstants.h"

#include "PointLight.h"

#include "Resources/Engine/Primitives/quad.h"
#include "Resources/Engine/Primitives/cube.h"

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
#include "TimeManager.h"
#include "StandardShader.h"


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
			shaderToUse->setValue("skybox", 0);
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
			shaderToUse->setValue("skybox", 0);
			shaderToUse->setValue("refractiveRatio", 1 / 1.52f);
			auto textures = context->getActiveScene()->getSkybox()->getTextureHandlers();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->bind();
		}
	}

	shaderToUse->setModelMatrix(m_transformation->getMatrix());
	auto elapsed = (float)Engine::get()->getTimeManager()->getElapsedTime(TimeManager::Duration::MilliSeconds) / 1000;
	shaderToUse->setTime(elapsed);

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

bool Model::attachShader(std::shared_ptr<StandardShader> shader)
{
	m_shader = shader;

	return true;
}

bool Model::attachShader(StandardShader* shader)
{
	m_shader = std::shared_ptr<StandardShader>(shader);

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
	return m_material->getTextureHandlers();
}

StandardShader* Model::getShader()
{
	return m_shader.get();
}

bool Model::setMaterial(std::shared_ptr<Material> material)
{
	m_material = material;

	return true;
}

std::shared_ptr<Material> Model::getMaterial()
{
	return m_material;
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
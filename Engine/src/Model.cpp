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
	Shader* currShader = m_shader.get();

	if (shader)
		currShader = shader;

	auto context = Engine::get()->getContext();
	if (context->getActiveScene()->getSkybox())
	{
		if (m_isReflective)
		{
			currShader = context->GetReflectionShader();
			currShader->use();
			currShader->setInt("skybox", 0);
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
			currShader = context->GetRefractiveShader();
			currShader->use();
			currShader->setInt("skybox", 0);
			currShader->setFloat("refractiveRatio", 1 / 1.52f);
			auto textures = context->getActiveScene()->getSkybox()->getTextureHandlers();
			if (textures.size() <= 0)
			{
				logError("Skybox does not contain cubemap texture.");
				return;
			}
			textures[0]->bind();
		}
	}

	currShader->setMat4("model", m_transformation->getTransformation());

	if (currShader->IsMaterialsEnabled() && m_material)
	{
		m_material->UseMaterial(*currShader);
	}

	renderer.SetDrawType(Renderer::DrawType::Triangles);

	for (auto i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->render(*currShader, renderer);
	}
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
	m_shader = Engine::get()->getRenderer()->GetDefaultShader();

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
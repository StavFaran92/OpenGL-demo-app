#include "Scene.h"

#include "Skybox.h"
#include "Object3D.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Engine.h"
#include "ICamera.h"
#include "Model.h"
#include "ObjectSelection.h"
#include "SkyboxRenderer.h"
#include "ScreenBufferProjector.h"

void Scene::init()
{
	m_renderer = std::make_shared<Renderer>();
	m_skyboxRenderer = std::make_shared<SkyboxRenderer>(*m_renderer.get());

	m_objectSelection = std::make_shared<ObjectSelection>();
	m_objectSelection->Init();

	m_screenBufferProjector = std::make_shared<ScreenBufferProjector>();
	if (!m_screenBufferProjector->Init())
	{
		logError("Screen buffer projector failed to init!");
	}

	std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();
	addDirectionalLight(light);
}

void Scene::update(float deltaTime)
{
	
	m_renderer->Clear();

	m_renderer->GetCamera()->update(deltaTime);

	// Update models
	for (auto model = m_models.begin(); model != m_models.end(); ++model)
	{
		model->second->Update(deltaTime);
	}

	if (m_skybox)
		m_skybox->Update(deltaTime);
}

void Scene::draw()
{
	if (m_isPostProcessEnabled && m_screenBufferProjector)
	{
		m_screenBufferProjector->RedirectToFrameBuffer();
	}

	// Draw models
	for (auto model = m_models.begin(); model != m_models.end(); ++model)
	{
		auto shader = model->second->GetShader();
		shader->UseShader();

		if (shader->IsLightsEnabled())
		{
			// Use all directional lights
			{
				int i = 0;
				for (auto it = m_directionalLights.begin(); it != m_directionalLights.end(); ++it, ++i) {
					it->second->useLight(shader, i);
				}
				shader->SetInt("dirLightCount", m_directionalLights.size());
			}

			// Use all point lights
			{
				int i = 0;
				for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++i, ++it) {
					it->second->useLight(shader, i);
				}
				shader->SetInt("pointLightCount", m_pointLights.size());
			}
		}

		glStencilFunc(GL_ALWAYS, model->second->getID(), 0xff);

		// Draw model
		model->second->Draw(m_renderer, shader);
	}

	if (m_skybox)
	{
		m_skybox->UseShader();
		m_skybox->Draw(m_skyboxRenderer);
	}

	if (m_isPostProcessEnabled && m_screenBufferProjector)
	{
		m_screenBufferProjector->RedirectToDefault();
	}
}

std::shared_ptr<ScreenBufferProjector> Scene::GetScreenBufferProjector() const
{
	return m_screenBufferProjector;
}


void Scene::clear()
{
	m_models.clear();
	m_pointLights.clear();
	m_directionalLights.clear();
}

bool Scene::addModel(std::shared_ptr<Model> model)
{
	m_modelCounter++;
	model->setID(m_modelCounter);
	m_models.emplace(m_modelCounter, model);

	logInfo("Model {} Added successfully.", std::to_string(m_modelCounter));
	return true;
}

bool Scene::addPointLight(std::shared_ptr<PointLight> pLight)
{
	m_pointLightCounter++;
	pLight->setID(m_pointLightCounter);
	m_pointLights.emplace(m_pointLightCounter, std::shared_ptr<PointLight>(pLight));

	logInfo("PointLight {} Added successfully.", std::to_string(m_pointLightCounter));
	return true;
}

bool Scene::addDirectionalLight(std::shared_ptr<DirectionalLight> dLight)
{
	m_directionalLightCounter++;
	dLight->setID(m_directionalLightCounter);
	m_directionalLights.emplace(m_directionalLightCounter, std::shared_ptr<DirectionalLight>(dLight));

	logInfo("DirectionalLight {} Added successfully.", std::to_string(m_directionalLightCounter));
	return true;
}

bool Scene::removeModel(uint32_t id)
{
	auto iter = m_models.find(id);
	if (iter == m_models.end())
	{
		logError("Could not locate Model {}", id);
		return false;
	}
	m_models.erase(iter);

	logInfo("Model {} Erased successfully.", std::to_string(id));

	return true;
}

bool Scene::removeModel(std::shared_ptr<Model> model)
{
		uint32_t id = model->getID();
		
		return removeModel(id);
}

bool Scene::removePointLight(std::shared_ptr<PointLight> pLight)
{
	uint32_t id = pLight->getID();
	auto iter = m_pointLights.find(id);
	if (iter == m_pointLights.end())
	{
		logError("Could not locate PointLight {}", id);
		return false;
	}
	m_pointLights.erase(iter);

	logInfo("PointLight {} Erased successfully.", std::to_string(id));

	return true;
}

bool Scene::removeDirectionalLight(std::shared_ptr<DirectionalLight> dLight)
{
	uint32_t id = dLight->getID();
	auto iter = m_directionalLights.find(id);
	if (iter == m_directionalLights.end())
	{
		logError("Could not locate DirectionalLight {}", id);
		return false;
	}
	m_directionalLights.erase(iter);

	logInfo("DirectionalLight {} Erased successfully.", std::to_string(id));

	return true;
}


void Scene::setSkybox(std::shared_ptr<Skybox> skybox)
{
	m_skybox = skybox;
}

void Scene::removeSkybox()
{
	m_skybox = nullptr;
}

std::shared_ptr<Renderer> Scene::getRenderer() const
{
	return m_renderer;
}

std::shared_ptr<Renderer> Scene::getSkyboxRenderer()
{
	return m_skyboxRenderer;
}

void Scene::close()
{
	clear();
}

void Scene::setPostProcess(bool value)
{
	m_isPostProcessEnabled = value;
}

std::shared_ptr<Skybox> Scene::getSkybox()
{
	return m_skybox;
}
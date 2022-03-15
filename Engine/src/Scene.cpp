#include "Scene.h"

#include "Skybox.h"
#include "Object3D.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Engine.h"
#include "ICamera.h"

void Scene::update(float deltaTime)
{
	
	Engine::Get()->GetRenderer()->Clear();

	Engine::Get()->GetRenderer()->GetCamera()->update(deltaTime);

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
}

void Scene::init()
{
}

void Scene::clear()
{
	m_models.clear();
	m_pointLights.clear();
	m_directionalLights.clear();
}

void Scene::addObject(std::shared_ptr<Object3D> object)
{
}

void Scene::removeObject(std::shared_ptr<Object3D> object)
{
}

void Scene::setSkybox(std::shared_ptr<Skybox> skybox)
{
	m_skybox = skybox;
}

void Scene::removeSkybox()
{
	m_skybox = nullptr;
}

void Scene::close()
{
	clear();
}

std::shared_ptr<Skybox> Scene::getSkybox()
{
	return m_skybox;
}
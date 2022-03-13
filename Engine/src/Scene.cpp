#include "Scene.h"

void Scene::update(float deltaTime)
{
	m_renderer->Clear();

	m_renderer->GetCamera()->update(deltaTime);

	// Update models
	auto iter = m_models.begin();
	while (iter != m_models.end())
	{
		iter->second->Update(deltaTime);
		iter++;
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
				shader->SetInt("dirLightCount", i);
			}

			// Use all point lights
			{
				int i = 0;
				for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++i, ++it) {
					it->second->useLight(shader, i);
				}
				shader->SetInt("pointLightCount", i);
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

std::shared_ptr<Skybox> Scene::getSkybox()
{
	return m_skybox;
}
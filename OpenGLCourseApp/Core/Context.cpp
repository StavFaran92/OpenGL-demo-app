#include "Context.h"

#include "Renderer/Geometry/Model.h"
#include "Renderer/Lighting/DirectionalLight.h"
#include "Renderer/Lighting/PointLight.h"

Context::Context() : m_modelCounter(0), m_shaderCounter(0)
{
	m_renderer = std::make_shared<Renderer>();

	std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();
	AddDirectionalLight(light);
}

bool Context::AddModel(std::shared_ptr<Model> model)
{
	m_modelCounter += 1;
	model->SetID(m_modelCounter);
	m_models.emplace(m_modelCounter, model);

	logInfo("Model {} Added successfully.", std::to_string(m_modelCounter));

	return true;
}

bool Context::RemoveModel(const uint32_t id)
{
	auto iter = m_models.find(id);
	if (iter == m_models.end())
	{
		logError("Could not locate model {}", id);
		return false;
	}

	m_models.erase(iter);

	logInfo("Model {} Erased successfully.", std::to_string(id));

	return true;
}

bool Context::AddShader(std::shared_ptr<Shader> shader)
{
	m_shaderCounter += 1;
	m_shaders.emplace(m_shaderCounter, shader);

	logInfo("Shader {} Added successfully.", std::to_string(m_shaderCounter));

	return true;
}

bool Context::RemoveShader(const uint32_t uid)
{
	auto iter = m_shaders.find(uid);
	if (iter == m_shaders.end())
	{
		logError("Could not locate shader {}", uid);
		return false;
	}

	m_shaders.erase(iter);

	logInfo("Shader {} Erased successfully.", std::to_string(uid));

	return true;
}

bool Context::AddPointLight(std::shared_ptr<PointLight> light)
{
	m_pointLightCounter += 1;
	m_pointLights.emplace(m_pointLightCounter, light);

	logInfo("Point Light {} Added successfully.", std::to_string(m_pointLightCounter));

	return true;
}

bool Context::RemovePointLight(const uint32_t uid)
{
	auto iter = m_pointLights.find(uid);
	if (iter == m_pointLights.end())
	{
		logError("Could not locate point light {}", uid);
		return false;
	}

	m_pointLights.erase(iter);

	logInfo("Point Light {} Erased successfully.", std::to_string(uid));

	return true;
}

bool Context::AddDirectionalLight(std::shared_ptr<DirectionalLight> light)
{
	m_directionalLightCounter += 1;
	m_directionalLights.emplace(m_directionalLightCounter, light);

	logInfo("Directional Light {} Added successfully.", std::to_string(m_directionalLightCounter));

	return true;
}

bool Context::RemoveDirectionalLight(const uint32_t uid)
{
	auto iter = m_directionalLights.find(uid);
	if (iter == m_directionalLights.end())
	{
		logError("Could not locate directional light {}", uid);
		return false;
	}

	m_directionalLights.erase(iter);

	logInfo("directional Light {} Erased successfully.", std::to_string(uid));

	return true;
}

void Context::Update(float deltaTime)
{
	// Update models
	auto iter = m_models.begin();
	while (iter != m_models.end())
	{
		iter->second->Update(deltaTime);
		iter++;
	}
}

void Context::Draw()
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
		model->second->Draw(m_renderer);
	}
}

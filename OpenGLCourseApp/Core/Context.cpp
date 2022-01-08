#include "Context.h"

#include "Renderer/Geometry/Model.h"

bool Context::AddModel(std::shared_ptr<Model> model)
{
	m_modelCounter += 1;
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
	auto iter = m_models.begin();
	while (iter != m_models.end())
	{
		iter->second->UseShader();
		iter->second->Draw(m_renderer);
		iter++;
	}
}

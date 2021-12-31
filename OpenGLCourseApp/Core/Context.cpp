#include "Context.h"

bool Context::AddModel(std::shared_ptr<Model> model)
{
	m_uid += 1;
	m_models.emplace(m_uid, model);

	logInfo("Model {} Added successfully.", std::to_string(m_uid));

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

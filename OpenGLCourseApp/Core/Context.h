#pragma once

#include <map>

#include "Renderer/Geometry/Model.h"
#include "Utils/Logger/Logger.h"

class Context
{
public:
	Context() : m_uid(0) 
	{}

	bool AddModel(std::shared_ptr<Model> model);
	bool RemoveModel(const uint32_t uid);
private:
	std::map<uint32_t, std::shared_ptr<Model>> m_models;
	uint32_t m_uid = 0;
};


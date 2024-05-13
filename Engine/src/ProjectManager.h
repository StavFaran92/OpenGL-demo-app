#pragma once

#include <string>

#include "Context.h"

class ProjectManager
{
public:
	std::shared_ptr<Context> loadProject(const std::string& filePath);
	void saveProject(const std::string& filePath);
};
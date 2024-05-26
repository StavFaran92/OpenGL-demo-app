#pragma once

#include <string>
#include <map>

#include "Core.h"
#include "Shader.h"
#include "Component.h"

class EngineAPI ShaderBuilder
{
public:
	static ShaderBuilder& create(const std::string& filePath);

	ShaderComponent build();

private:
	ShaderBuilder(const std::string& filePath);

private:
	std::string m_filepath;
};
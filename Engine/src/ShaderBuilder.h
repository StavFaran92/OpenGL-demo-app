#pragma once

#include <string>

#include "Core.h"
#include "Shader.h"

class EngineAPI ShaderBuilder
{
public:
	enum class UniformType
	{
		FLOAT,
		INT,
		BOOL,
		VEC2,
		VEC3,
		VEC4,
		MAT3,
		MAT4
	};

public:
	static ShaderBuilder& create(const std::string& filePath);

	ShaderBuilder& addUniform(const std::string& name, UniformType uType);

	Shader* build();

private:
	ShaderBuilder(const std::string& filePath);
};
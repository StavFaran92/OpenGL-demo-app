#pragma once

#include <string>
#include <memory>

#include "Shader.h"
#include "Core.h"

class EngineAPI ShaderBuilder
{
public:
	template<typename T, typename... _Types>
	static T* buildShader(_Types&&... _Args)
	{
		//static_assert(std::is_base_of<Shader, T>::value, "T must be a type derived from Shader");

		auto shader = new T(std::forward<_Types>(_Args)...);
		shader->init();

		return shader;
	}

	template<typename T, typename... _Types>
	static std::shared_ptr<T> buildShaderShared(_Types&&... _Args)
	{
		//static_assert(std::is_base_of<Shader, T>::value, "T must be a type derived from Shader");

		auto shader = std::make_shared<T>(std::forward<_Types>(_Args)...);
		shader->init();

		return shader;
	}
};
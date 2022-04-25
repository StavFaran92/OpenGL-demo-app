#pragma once
#include <string>

#include "Shader.h"

class PhongShader : public Shader
{
public:
	PhongShader();

	void setDirLightCount(int count);
	void setPointLightCount(int count);
};


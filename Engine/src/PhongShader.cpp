#include "PhongShader.h"

PhongShader::PhongShader() : Shader("Resources\\Shaders\\LightShader.vert", "Resources\\Shaders\\LightShader.frag")
{
}

void PhongShader::setDirLightCount(int count)
{
	setInt("dirLightCount", count);
}

void PhongShader::setPointLightCount(int count)
{
	setInt("pointLightCount", count);
}

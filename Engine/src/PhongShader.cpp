#include "PhongShader.h"

PhongShader::PhongShader() : Shader("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag")
{
	m_enableLight = true;
	m_enableTexture = true;
	m_enableMaterial = true;
}

void PhongShader::setDirLightCount(int count)
{
	setInt("dirLightCount", count);
}

void PhongShader::setPointLightCount(int count)
{
	setInt("pointLightCount", count);
}

#include "PhongShader.h"

#include "PointLight.h"
#include "DirectionalLight.h"

PhongShader::PhongShader() : Shader("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag")
{
	m_enableLight = true;
	m_enableTexture = true;
	m_enableMaterial = true;
	m_enableColors = true;
}

void PhongShader::setDirLightCount(int count)
{
	setInt("dirLightCount", count);
}

void PhongShader::setPointLightCount(int count)
{
	setInt("pointLightCount", count);
}

void PhongShader::updateDirLights(std::unordered_map<uint32_t, std::shared_ptr<DirectionalLight>>& dirLights)
{
	// Use all point lights
	int i = 0;
	for (auto it = dirLights.begin(); it != dirLights.end(); ++i, ++it) {
		it->second->useLight(*this, i);
	}
	setDirLightCount(dirLights.size());
}

void PhongShader::updatePointLights(std::unordered_map<uint32_t, std::shared_ptr<PointLight>>& pointLights)
{
	// Use all directional lights
	int i = 0;
	for (auto it = pointLights.begin(); it != pointLights.end(); ++it, ++i) {
		it->second->useLight(*this, i);
	}
	setPointLightCount(pointLights.size());
}

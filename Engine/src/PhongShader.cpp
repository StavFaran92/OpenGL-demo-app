#include "PhongShader.h"

#include "PointLight.h"
#include "DirectionalLight.h"

PhongShader::PhongShader() : Shader("Resources/Engine/Shaders/shader.vert", "Resources/Engine/Shaders/shader.frag")
{
	m_enableLight = true;
	m_enableTexture = true;
	m_enableMaterial = true;
	m_enableColors = true;
}

void PhongShader::setDirLightCount(int count)
{
	setValue("dirLightCount", count);
}

void PhongShader::setPointLightCount(int count)
{
	setValue("pointLightCount", count);
}

void PhongShader::setUseColors(bool enable)
{
	setValue("useColors", enable);
}
void PhongShader::setViewPos(glm::vec3 viewPosition)
{
	setValue("viewPos", viewPosition);
}

void PhongShader::setColorMul(glm::vec4 colorMul)
{
	setValue("colorMul", colorMul);
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

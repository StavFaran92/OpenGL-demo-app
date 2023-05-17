#include "StandardShader.h"

#include "DirectionalLight.h"
#include "PointLight.h"

StandardShader::StandardShader(const std::string& vertexfilePath, const std::string& fragmentFilePath, const std::string& geometryShader/* = ""*/)
	: Shader(vertexfilePath, fragmentFilePath, geometryShader)
{
}


void StandardShader::setDirLightCount(int count)
{
	if (m_enableLight)
	{
		setInt("dirLightCount", count);
	}
}

void StandardShader::setPointLightCount(int count)
{
	if (m_enableLight)
	{
		setInt("pointLightCount", count);
	}
}

void StandardShader::setUseColors(bool enable)
{
	if (m_enableColors)
	{
		setInt("useColors", enable);
	}
}
void StandardShader::setViewPos(glm::vec3 viewPosition)
{
	setFloat("viewPos", viewPosition);
}

void StandardShader::setColorMul(glm::vec4 colorMul)
{
	setFloat("colorMul", colorMul);
}

void StandardShader::updateDirLights(std::unordered_map<uint32_t, std::shared_ptr<DirectionalLight>>& dirLights)
{
	if (m_enableLight)
	{
		// Use all point lights
		int i = 0;
		for (auto it = dirLights.begin(); it != dirLights.end(); ++i, ++it) {
			it->second->useLight(*this, i);
		}
		setDirLightCount(dirLights.size());
	}
}

void StandardShader::updatePointLights(std::unordered_map<uint32_t, std::shared_ptr<PointLight>>& pointLights)
{
	if (m_enableLight)
	{
		// Use all directional lights
		int i = 0;
		for (auto it = pointLights.begin(); it != pointLights.end(); ++it, ++i) {
			it->second->useLight(*this, i);
		}
		setPointLightCount(pointLights.size());
	}
}
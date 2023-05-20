#include "StandardShader.h"

#include "DirectionalLight.h"
#include "PointLight.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "ShaderExtender.h"

StandardShader::StandardShader(const std::string& vertexfilePath, const std::string& fragmentFilePath, const std::string& geometryShader/* = ""*/)
	: Shader(vertexfilePath, fragmentFilePath, geometryShader)
{
	m_enableMaterial = true;
	m_enableTexture = true;
	m_enableLight = true;
}


void StandardShader::setDirLightCount(int count)
{
	if (m_enableLight)
	{
		setValue("dirLightCount", count);
	}
}

void StandardShader::setPointLightCount(int count)
{
	if (m_enableLight)
	{
		setValue("pointLightCount", count);
	}
}

void StandardShader::setUseColors(bool enable)
{
	if (m_enableColors)
	{
		setValue("useColors", enable);
	}
}
void StandardShader::setViewPos(glm::vec3 viewPosition)
{
	setValue("viewPos", viewPosition);
}

void StandardShader::setColorMul(glm::vec4 colorMul)
{
	setValue("colorMul", colorMul);
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

void StandardShader::BuildShaders(const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryCode)
{
	//Create a new shader program
	m_id = glCreateProgram();

	// Validate shader program creation
	if (!ValidateRenderer())
		return;

	// Create and attach vertex shader to program
	GLuint vertexShader = AddShader(vertexCode, GL_VERTEX_SHADER);

	//ShaderExtender::ExtensionParams eParams{};
	//std::string extendedVertexCode = ShaderExtender::ExtendVertexShader(vertexCode, eParams);
	//GLuint extendedVertexShader = AddShader(vertexCode, GL_VERTEX_SHADER);
	glAttachShader(m_id, vertexShader);

	GLuint geometryShader = 0;
	if (!geometryCode.empty())
	{
		// Create and attach geometry shader to program
		geometryShader = AddShader(geometryCode, GL_GEOMETRY_SHADER);
		glAttachShader(m_id, geometryShader);
	}

	ShaderExtender::ExtensionParams eParams{};
	eParams.enableLight = true;
	eParams.enableMaterial = true;
	std::string extendedFragmentCode = ShaderExtender::ExtendFragmentShader(fragmentCode, eParams);

	// Create and attach fragment shader to program
	GLuint fragShader = AddShader(extendedFragmentCode, GL_FRAGMENT_SHADER);
	glAttachShader(m_id, fragShader);

	// Link shader program
	glLinkProgram(m_id);

	// Validate shader program link
	ValidateProgramLink();

	// Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	if (geometryShader)
		glDeleteShader(geometryShader);
}
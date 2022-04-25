#include "Light.h"
#include <GL/glew.h>
#include "ApplicationConstants.h"
#include "Transformation.h"
#include "Shader.h"
#include "Logger.h"

Light::Light()
	:m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_ambientIntensity(0.2f), m_diffuseIntensity(.5f)
{
	m_transform = std::make_shared<Transformation>();
}

Light::Light(glm::vec3 color, float aIntensity, float dIntensity) :
	m_color(color), m_ambientIntensity(aIntensity), m_diffuseIntensity(dIntensity)
{
	m_transform = std::make_shared<Transformation>();
}

void Light::useLight(Shader& shader, int index)
{
	if (!shader.IsLightsEnabled())
	{
		logError("Shader does not support light");
		return;
	}

	shader.setFloat(m_name + "[" +std::to_string(index) + "]" + ".color", m_color);
	shader.setFloat(m_name + "[" +std::to_string(index) + "]" + ".ambient", Constants::VEC3_ONE * m_ambientIntensity);
	shader.setFloat(m_name + "[" +std::to_string(index) + "]" + ".diffuse", Constants::VEC3_ONE * m_diffuseIntensity); // darken diffuse light a bit
	shader.setFloat(m_name + "[" +std::to_string(index) + "]" + ".specular", Constants::VEC3_ONE);
}

void Light::SetAmbientIntensity(float intensity)
{
	m_ambientIntensity = intensity;
}

void Light::SetDiffuseIntensity(float intensity)
{
	m_diffuseIntensity = intensity;
}

void Light::SetColor(glm::vec3 color)
{
	m_color = color;
}

Light::~Light()
{
}
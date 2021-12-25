#include "Light.h"

Light::Light()
	:m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_ambientIntensity(1), m_diffuseIntensity(0.0f)
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) :
	m_color(glm::vec3(red, green, blue)), m_ambientIntensity(aIntensity), m_diffuseIntensity(dIntensity)
{
}

void Light::useLight(Shader& shader)
{
	shader.SetFloat("directionalLight.color", m_color);
	shader.SetFloat("directionalLight.ambientIntensity", m_ambientIntensity);
	shader.SetFloat("directionalLight.diffuseIntensity", m_diffuseIntensity);
}

Light::~Light()
{
}
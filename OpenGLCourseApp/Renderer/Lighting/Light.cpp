#include "Light.h"

Light::Light()
	:m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_ambientIntensity(1.0f), m_diffuseIntensity(1.0f)
{
	m_transform = std::make_shared<Transform>();
}

Light::Light(glm::vec3 color, GLfloat aIntensity, GLfloat dIntensity) :
	m_color(color), m_ambientIntensity(aIntensity), m_diffuseIntensity(dIntensity)
{
}

void Light::useLight(std::shared_ptr<Shader> shader)
{
	shader->SetFloat(m_name + ".color", m_color);
	shader->SetFloat(m_name + ".ambient", VEC3_ONE * m_ambientIntensity);
	shader->SetFloat(m_name + ".diffuse", VEC3_ONE * m_diffuseIntensity); // darken diffuse light a bit
	shader->SetFloat(m_name + ".specular", VEC3_ONE);
}

Light::~Light()
{
}
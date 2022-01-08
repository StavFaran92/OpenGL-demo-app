#include "Light.h"

Light::Light()
	:m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_ambientIntensity(0.2f), m_diffuseIntensity(.5f)
{
	m_transform = std::make_shared<Transform>();
}

Light::Light(glm::vec3 color, GLfloat aIntensity, GLfloat dIntensity) :
	m_color(color), m_ambientIntensity(aIntensity), m_diffuseIntensity(dIntensity)
{
	m_transform = std::make_shared<Transform>();
}

void Light::useLight(std::shared_ptr<Shader> shader, int index)
{
	shader->SetFloat(m_name + "[" +std::to_string(index) + "]" + ".color", m_color);
	shader->SetFloat(m_name + "[" +std::to_string(index) + "]" + ".ambient", VEC3_ONE * m_ambientIntensity);
	shader->SetFloat(m_name + "[" +std::to_string(index) + "]" + ".diffuse", VEC3_ONE * m_diffuseIntensity); // darken diffuse light a bit
	shader->SetFloat(m_name + "[" +std::to_string(index) + "]" + ".specular", VEC3_ONE);
}

Light::~Light()
{
}
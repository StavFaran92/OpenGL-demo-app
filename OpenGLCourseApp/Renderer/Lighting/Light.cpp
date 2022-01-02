#include "Light.h"

Light::Light()
	:m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_ambientIntensity(1.0f), m_diffuseIntensity(1.0f)
{
	m_transform = std::make_shared<Transform>();
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) :
	m_color(glm::vec3(red, green, blue)), m_ambientIntensity(aIntensity), m_diffuseIntensity(dIntensity)
{
}

void Light::useLight(std::shared_ptr<Shader> shader)
{
	shader->SetFloat("light.color", m_color);
	shader->SetFloat("light.position", m_transform->GetPosition());
	shader->SetFloat("light.ambient", Constants::VEC3_ONE * m_ambientIntensity);
	shader->SetFloat("light.diffuse", Constants::VEC3_ONE * m_diffuseIntensity); // darken diffuse light a bit
	shader->SetFloat("light.specular", Constants::VEC3_ONE);
}

Light::~Light()
{
}
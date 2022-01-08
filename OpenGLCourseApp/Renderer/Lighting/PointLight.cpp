#include "PointLight.h"

uint32_t PointLight::m_count = 0;

void PointLight::Init()
{
	m_count++;
	m_name = "pointLights[" + std::to_string(m_count-1) + "]";
}

void PointLight::useLight(std::shared_ptr<Shader >shader)
{
	Light::useLight(shader);

	shader->SetFloat(m_name + ".position", m_transform->GetPosition());
	shader->SetFloat(m_name + ".constant", constant);
	shader->SetFloat(m_name + ".linear", linear);
	shader->SetFloat(m_name + ".quadratic", quadratic);
}
#include "PointLight.h"

void PointLight::useLight(std::shared_ptr<Shader >shader, int index)
{
	Light::useLight(shader, index);

	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".position", m_transform->GetPosition());
	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".constant", constant);
	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".linear", linear);
	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".quadratic", quadratic);
}
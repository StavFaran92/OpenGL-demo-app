#include "PointLight.h"

void PointLight::useLight(std::shared_ptr<Shader >shader, int index)
{
	if (!shader->IsLightsEnabled())
	{
		logError("Shader does not support light");
		return;
	}

	Light::useLight(shader, index);

	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".position", m_transform->GetPosition());
	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".constant", m_attenuation.constant);
	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".linear", m_attenuation.linear);
	shader->SetFloat(m_name + "["+std::to_string(index) +"]"+ ".quadratic", m_attenuation.quadratic);
}
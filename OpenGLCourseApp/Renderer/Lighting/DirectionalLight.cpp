#include "DirectionalLight.h"

void DirectionalLight::useLight(std::shared_ptr<Shader> shader, int index)
{
	if (!shader->IsLightsEnabled())
	{
		logError("Shader does not support light");
		return;
	}

	Light::useLight(shader, index);

	shader->SetFloat(m_name + "[" + std::to_string(index) + "]" + ".direction", m_direction);
}
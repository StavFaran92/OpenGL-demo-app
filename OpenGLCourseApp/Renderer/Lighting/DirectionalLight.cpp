#include "DirectionalLight.h"

void DirectionalLight::useLight(std::shared_ptr<Shader> shader, int index)
{
	Light::useLight(shader, index);

	shader->SetFloat(m_name + "[" + std::to_string(index) + "]" + ".direction", m_direction);
}
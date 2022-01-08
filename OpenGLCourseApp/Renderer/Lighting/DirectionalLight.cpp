#include "DirectionalLight.h"

void DirectionalLight::useLight(std::shared_ptr<Shader> shader)
{
	Light::useLight(shader);

	shader->SetFloat(m_name + ".direction", m_direction);
}
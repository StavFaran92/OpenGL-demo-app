#include "DirectionalLight.h"

void DirectionalLight::useLight(Shader& shader)
{
	Light::useLight(shader);

	shader.SetFloat("directionalLight.direction", m_direction);
}
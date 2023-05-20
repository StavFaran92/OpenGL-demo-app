#include "DirectionalLight.h"

#include "Shader.h"
#include "Logger.h"

void DirectionalLight::useLight(Shader& shader, int index)
{
	Light::useLight(shader, index);

	shader.setValue(m_name + "[" + std::to_string(index) + "]" + ".direction", m_direction);
}
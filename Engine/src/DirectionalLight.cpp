#include "DirectionalLight.h"

#include "Shader.h"
#include "Logger.h"

void DirectionalLight::useLight(Shader& shader, int index)
{
	Light::useLight(shader, index);

	shader.setValue(m_name + "[" + std::to_string(index) + "]" + ".direction", m_direction);
}

glm::vec3 DirectionalLight::getDirection() const
{
	return m_direction;
}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	m_direction = dir;
}

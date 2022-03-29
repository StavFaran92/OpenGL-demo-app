#include "Transform.h"
#include<glm/gtc/quaternion.hpp>
#include "LinearAlgebraUtil.h"

void Transform::update(float deltaTime)
{
	if (m_change)
	{
		//m_transformation = glm::mat4(1.0f);
		m_transformation = glm::mat4_cast(m_orientation);
		m_transformation = glm::translate(m_transformation, m_translation);
		m_transformation = glm::scale(m_transformation, m_scale);

		m_change = false;
	}
}
void Transform::SetPosition(glm::vec3 pos)
{
	m_translation = pos;

	m_change = true;
}
void Transform::SetRotation(float angle, glm::vec3 axis)
{
	m_change = true;
}
void Transform::SetScale(glm::vec3 scale)
{
	m_scale = scale;

	m_change = true;
}

glm::quat Transform::getOrientation() const
{
	return m_orientation;
}

void Transform::translate(float x, float y, float z)
{
	m_translation.x += x;
	m_translation.y += y;
	m_translation.z += z;

	m_change = true;
}

void Transform::rotateX(float angle)
{
	m_orientation = glm::rotate(m_orientation, degToRad(angle), {1,0,0});

	m_change = true;
}

void Transform::rotateY(float angle)
{
	m_orientation = glm::rotate(m_orientation, degToRad(angle), { 0,1,0 });

	m_change = true;
}

void Transform::rotateZ(float angle)
{
	m_orientation = glm::rotate(m_orientation, degToRad(angle), { 0,0, 1 });

	m_change = true;
}

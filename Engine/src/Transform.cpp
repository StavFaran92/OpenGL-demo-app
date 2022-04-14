#include "Transform.h"
#include<glm/gtc/quaternion.hpp>
#include "LinearAlgebraUtil.h"

glm::mat4 Transformation::getTransformation() const
{
	return m_transformation;
}

void Transformation::update(float deltaTime)
{
	if (m_change)
	{
		auto a = glm::translate(glm::mat4(1.0f), {1,1,1});
		auto b = glm::translate(glm::mat4(1.0f), {-1,-1,-1});

		glm::mat4 identity(1.0f);
		auto translate = glm::translate(glm::mat4(1.0f), m_translation);
		auto rotate = glm::mat4_cast(m_orientation);
		auto scale = glm::scale(glm::mat4(1.0f), m_scale);

		m_transformation = a * scale * rotate * b * translate * identity;

		m_change = false;
	}
}
void Transformation::SetPosition(glm::vec3 pos)
{
	m_translation = pos;

	m_change = true;
}
void Transformation::SetRotation(float angle, glm::vec3 axis)
{
	m_change = true;
}
void Transformation::SetScale(glm::vec3 scale)
{
	m_scale = scale;

	m_change = true;
}

glm::vec3 Transformation::getPosition() const
{
	return m_translation;
}

glm::quat Transformation::getOrientation() const
{
	return m_orientation;
}

glm::vec3 Transformation::getScale() const
{
	return m_scale;
}

void Transformation::translate(float x, float y, float z)
{
	m_translation.x += x;
	m_translation.y += y;
	m_translation.z += z;

	m_change = true;
}

void Transformation::rotateLocalX(float angle)
{
	m_orientation = glm::rotate(m_orientation, degToRad(angle), {1,0,0});

	m_change = true;
}

void Transformation::rotateLocalY(float angle)
{
	m_orientation = glm::rotate(m_orientation, degToRad(angle), { 0,1,0 });

	m_change = true;
}

void Transformation::rotateLocalZ(float angle)
{
	m_orientation = glm::rotate(m_orientation, degToRad(angle), { 0,0, 1 });

	m_change = true;
}

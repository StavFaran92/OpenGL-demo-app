#include "Transformation.h"
#include<glm/gtx/quaternion.hpp>
#include "LinearAlgebraUtil.h"

glm::mat4 Transformation::getMatrix() const
{
	return m_transformation;
}

void Transformation::getMatrix(glm::mat4& mat)
{
	mat = m_transformation;
}

void Transformation::addChild(Transformation* transform)
{
	m_children.push_back(transform);
}

void Transformation::removeChild(Transformation* transform)
{
	auto it = std::find(m_children.begin(), m_children.end(), transform);

	// If the element is found, remove it.
	if (it != m_children.end())
	{
		m_children.erase(it);
	}
}

void Transformation::update(float deltaTime)
{
	if (m_change)
	{

		glm::mat4 identity(1.0f);
		auto translate = glm::translate(glm::mat4(1.0f), m_translation);
		auto scale = glm::scale(glm::mat4(1.0f), m_scale);

		auto finalRotation = glm::mat4_cast(m_orientationLocal) * m_relativeRot * identity;

		m_transformation = scale * finalRotation * translate * identity;

		m_change = false;
	}
}
void Transformation::setPosition(glm::vec3 pos)
{
	m_translation = pos;

	m_change = true;
}
void Transformation::setRotation(float angle, glm::vec3 axis)
{
	m_change = true;
}
void Transformation::setScale(glm::vec3 scale)
{
	m_scale = scale;

	m_change = true;
}

glm::vec3 Transformation::getPosition() const
{
	return m_translation;
}

glm::quat Transformation::getLocalOrientation() const
{
	return m_orientationLocal;
}

glm::quat Transformation::getWorldOrientation() const
{
	return m_orientationWorld;
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

void Transformation::translate(glm::vec3 translation)
{
	translate(translation.x, translation.y, translation.z);
}

void Transformation::scale(float x, float y, float z)
{
	m_scale.x *= x;
	m_scale.y *= y;
	m_scale.z *= z;

	m_change = true;
}

void Transformation::scale(glm::vec3 scaleFactor)
{
	scale(scaleFactor.x, scaleFactor.y, scaleFactor.z);
}

void Transformation::rotate(glm::vec3 eulers)
{
	m_orientationLocal = glm::quat(eulers) * m_orientationLocal;

	m_change = true;
}

void Transformation::rotateLerp(glm::vec3 axis, float angle, float t)
{
	m_orientationLocal = glm::mix(m_orientationLocal, glm::angleAxis(degToRad(angle), axis) * m_orientationLocal, t);// *m_orientationLocal;

	m_change = true;
}


void Transformation::rotate(glm::vec3 axis, float angle)
{
	m_orientationLocal = glm::angleAxis(degToRad(angle), axis) * m_orientationLocal;

	m_change = true;
}

void Transformation::rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle)
{
	auto p = glm::translate(glm::mat4(1.0f), pivot);
	auto pInv = glm::translate(glm::mat4(1.0f), -pivot);

	m_relativeRot = p * glm::mat4_cast(glm::angleAxis(degToRad(angle), axis)) * pInv * m_relativeRot;

	m_change = true;
}


void Transformation::rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t)
{
	auto p = glm::translate(glm::mat4(1.0f), pivot);
	auto pInv = glm::translate(glm::mat4(1.0f), -pivot);

	m_relativeRot = p * glm::mat4_cast(glm::mix(glm::toQuat(m_relativeRot), glm::angleAxis(degToRad(angle), axis) * glm::toQuat(m_relativeRot), t)) * pInv;// *m_relativeRot;

	m_change = true;
}
#include "Transformation.h"
#include<glm/gtx/quaternion.hpp>
#include "LinearAlgebraUtil.h"
#include "Logger.h"

glm::mat4 Transformation::getMatrix() 
{
	update(0);

	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		return parentTransform.getMatrix() * m_transformation;
	}
	else
	{
		return m_transformation;
	}
}

void Transformation::getMatrix(glm::mat4& mat)
{
	update(0);

	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		mat = parentTransform.getMatrix() * m_transformation;
	}
	else
	{
		mat = m_transformation;
	}

}

void Transformation::setParent(Entity parent)
{
	if (m_parent != Entity::EmptyEntity)
	{
		removeParent();
	}

	auto& pTransform = parent.getComponent<Transformation>();
	m_rootTransformation = glm::inverse(pTransform.getMatrix());

	m_parent = parent;
	parent.addChildren(m_entity);
}

void Transformation::removeParent()
{
	auto& pTransform = m_parent.getComponent<Transformation>();
	m_rootTransformation = glm::mat4(1);

	m_parent = Entity::EmptyEntity;
}

Entity Transformation::getParent() const
{
	return m_parent;
}

void Transformation::addChild(Entity entity)
{
	m_children[entity.handlerID()] = entity;
}

void Transformation::removeChild(Entity entity)
{
	m_children.erase(entity.handlerID());
}

std::unordered_map<entity_id, Entity> Transformation::getChildren()
{
	return m_children;
}

void Transformation::update(float deltaTime)
{
	if (m_change)
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_translation);
		glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

		m_transformation = m_rootTransformation * translationMatrix * rotationMatrix * scaleMatrix;
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
void Transformation::setRotation(glm::quat quat)
{
	m_orientation = quat;

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
	m_orientation = glm::quat(eulers) * m_orientation;

	m_change = true;
}

void Transformation::rotateLerp(glm::vec3 axis, float angle, float t)
{
	m_orientation = glm::mix(m_orientation, glm::angleAxis(degToRad(angle), axis) * m_orientation, t);// *m_orientationLocal;

	m_change = true;
}


void Transformation::rotate(glm::vec3 axis, float angle)
{
	m_orientation = glm::angleAxis(degToRad(angle), axis) * m_orientation;

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
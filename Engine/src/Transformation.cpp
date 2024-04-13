#include "Transformation.h"
#include<glm/gtx/quaternion.hpp>
#include "LinearAlgebraUtil.h"
#include "Logger.h"

glm::mat4 Transformation::getWorldTransformation() 
{
	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		return parentTransform.getWorldTransformation() * getLocalTransformation();
	}
	else
	{
		return getLocalTransformation();
	}
}

void Transformation::getWorldTransformation(glm::mat4& mat)
{
	mat = getWorldTransformation();
}

glm::mat4 Transformation::getLocalTransformation()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_translation);
	glm::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

	return translationMatrix * rotationMatrix * scaleMatrix;
}

void Transformation::setParent(Entity parent)
{
	if (m_parent != Entity::EmptyEntity)
	{
		removeParent();
	}

	auto& pTransform = parent.getComponent<Transformation>();
	//m_rootTransformation = glm::inverse(pTransform.getWorldTransformation());

	setLocalPosition(getLocalPosition() - pTransform.getWorldPosition());
	setLocalRotation(glm::inverse(pTransform.getLocalRotation()) * m_rotation);
	setLocalScale(getLocalScale() / pTransform.getWorldScale());

	m_parent = parent;
	pTransform.addChild(m_entity);
	//parent.addChildren(m_entity);
}

void Transformation::removeParent()
{
	if (m_parent == Entity::EmptyEntity)
		return;

	//m_parent.removeChildren(m_entity);
	auto& pTransform = m_parent.getComponent<Transformation>();
	pTransform.removeChild(m_entity);

	m_parent = Entity::EmptyEntity;
}

Entity Transformation::getParent() const
{
	return m_parent;
}

Entity Transformation::getRoot() const
{
	return m_root;
}

Entity Transformation::setRoot(Entity root)
{
	return m_root = root;
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

void Transformation::setLocalPosition(glm::vec3 pos)
{
	m_translation = pos;
}
void Transformation::setWorldPosition(glm::vec3 pos)
{
	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		setLocalPosition(pos - parentTransform.getWorldPosition());
	}
	else
	{
		setLocalPosition(pos);
	}
}
void Transformation::setLocalRotation(float angle, glm::vec3 axis)
{
	throw std::runtime_error("Not yet implemented");
	//m_change = true;
}
void Transformation::setLocalRotation(glm::quat quat)
{
	m_rotation = quat;
}
void Transformation::setWorldRotation(glm::quat quat)
{
	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		setLocalRotation(glm::inverse(parentTransform.getWorldRotation()) * quat);
	}
	else
	{
		setLocalRotation(quat);
	}
}
void Transformation::setLocalScale(glm::vec3 scale)
{
	m_scale = scale;
}

glm::vec3 Transformation::getLocalPosition() const
{
	return m_translation;
}

glm::vec3 Transformation::getWorldPosition() const
{
	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		return parentTransform.getWorldPosition() + m_translation;
	}
	else
	{
		return m_translation;
	}
}

glm::quat Transformation::getLocalRotation() const
{
	return m_rotation;
}

glm::quat Transformation::getWorldRotation() const
{
	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		return parentTransform.getWorldRotation() * m_rotation;
	}
	else
	{
		return m_rotation;
	}
}

glm::vec3 Transformation::getLocalScale() const
{
	return m_scale;
}

glm::vec3 Transformation::getWorldScale() const
{
	auto parent = m_entity.getParent();
	if (parent.valid())
	{
		auto& parentTransform = parent.getComponent<Transformation>();
		return parentTransform.getWorldScale() * m_scale;
	}
	else
	{
		return m_scale;
	}
}

void Transformation::translate(float x, float y, float z)
{
	m_translation.x += x;
	m_translation.y += y;
	m_translation.z += z;
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
}

void Transformation::scale(glm::vec3 scaleFactor)
{
	scale(scaleFactor.x, scaleFactor.y, scaleFactor.z);
}

void Transformation::rotate(glm::vec3 eulers)
{
	m_rotation = glm::quat(eulers) * m_rotation;
}

void Transformation::rotateLerp(glm::vec3 axis, float angle, float t)
{
	m_rotation = glm::mix(m_rotation, glm::angleAxis(degToRad(angle), axis) * m_rotation, t);// *m_orientationLocal;
}


void Transformation::rotate(glm::vec3 axis, float angle)
{
	m_rotation = glm::angleAxis(degToRad(angle), axis) * m_rotation;
}

void Transformation::rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle)
{
	auto p = glm::translate(glm::mat4(1.0f), pivot);
	auto pInv = glm::translate(glm::mat4(1.0f), -pivot);

	m_relativeRot = p * glm::mat4_cast(glm::angleAxis(degToRad(angle), axis)) * pInv * m_relativeRot;
}


void Transformation::rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t)
{
	auto p = glm::translate(glm::mat4(1.0f), pivot);
	auto pInv = glm::translate(glm::mat4(1.0f), -pivot);

	m_relativeRot = p * glm::mat4_cast(glm::mix(glm::toQuat(m_relativeRot), glm::angleAxis(degToRad(angle), axis) * glm::toQuat(m_relativeRot), t)) * pInv;// *m_relativeRot;
}
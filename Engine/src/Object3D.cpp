#include "Object3D.h"

#include "Transformation.h"
#include "Logger.h"
#include "Engine.h"
#include "ObjectManager.h"

Object3D::Object3D()
{
	m_transformation = std::make_shared<Transformation>();

	Engine::get()->getObjectManager()->addObject(this);
}

void Object3D::update(float deltatime)
{
	m_transformation->update(deltatime);
}

void Object3D::onSelected()
{
}

void Object3D::onPicked()
{
}

Transformation* Object3D::getTransformation() const
{ 
	return m_transformation.get(); 
}

uint32_t Object3D::getID() const
{ 
	return m_id; 
}

void Object3D::translate(float x, float y, float z) const
{
	m_transformation->translate(x, y, z);

	for (const auto& child : m_childrens)
	{
		if (!child)
			continue;

		child->translate(x,y,z);
	}
}

void Object3D::translate(glm::vec3 translation) const
{
	m_transformation->translate(translation);
}

void Object3D::scale(float x, float y, float z) const
{
	m_transformation->scale(x, y, z);
}

void Object3D::scale(glm::vec3 scaleFactor) const
{
	m_transformation->scale(scaleFactor);
}

void Object3D::rotate(glm::vec3 eulers) const
{
	m_transformation->rotate(eulers);

	for (const auto& child : m_childrens)
	{
		if (!child)
			continue;

		child->rotate(eulers);
	}
}

void Object3D::rotate(glm::vec3 axis, float angle) const
{
	m_transformation->rotate(axis, angle);

	for (const auto& child : m_childrens)
	{
		if (!child)
			continue;

		child->rotateAround(m_transformation->getPosition(), axis, angle);
	}
}

void Object3D::rotateLerp(glm::vec3 axis, float angle, float t) const
{
	m_transformation->rotateLerp(axis, angle, t);

	for (const auto& child : m_childrens)
	{
		if (!child)
			continue;

		child->rotateAroundLerp(m_transformation->getPosition(), axis, angle, t);
	}
}

void Object3D::rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle) const
{
	m_transformation->rotateAround(pivot, axis, angle);

	for (const auto& child : m_childrens)
	{
		if (!child)
			continue;

		child->rotateAround(pivot, axis, angle);
	}
}

void Object3D::rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t) const
{
	m_transformation->rotateAroundLerp(pivot, axis, angle, t);

	for (const auto& child : m_childrens)
	{
		if (!child)
			continue;

		child->rotateAroundLerp(pivot, axis, angle, t);
	}
}

void Object3D::addChildren(std::shared_ptr<Object3D> child)
{
	m_childrens.emplace_back(child.get());
}

void Object3D::addChildren(Object3D* child)
{
	m_childrens.emplace_back(child);
}

void Object3D::removeChildren(std::shared_ptr<Object3D> child)
{
	removeChildren(child.get());
}

void Object3D::removeChildren(Object3D* child)
{
	auto iter = std::find(m_childrens.begin(), m_childrens.end(), child);
	if (iter == m_childrens.end())
	{
		logDebug("Could not locate " + std::to_string(child->getID()) + " in object childrens.");
		return;
	}

	m_childrens.erase(iter);
}

void Object3D::setSceneID(uint32_t id)
{
	m_sceneID = id;
}

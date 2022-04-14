#include "Object3D.h"

#include "Transformation.h"
#include "Logger.h"

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

void Object3D::setID(uint32_t id)
{
	m_id = id;
}

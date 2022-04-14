#include "Object3D.h"

#include "Transformation.h"

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
}

void Object3D::rotate(glm::vec3 eulers) const
{
	m_transformation->rotate(eulers);
}

void Object3D::rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle) const
{
	m_transformation->rotateAround(pivot, axis, angle);
}

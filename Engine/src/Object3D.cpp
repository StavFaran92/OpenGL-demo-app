#include "Object3D.h"

#include "Transform.h"

Transform& Object3D::GetTransformation() const
{ 
	return *m_transformation.get(); 
}

uint32_t Object3D::getID() const
{ 
	return m_id; 
}

void Object3D::translate(float x, float y, float z)
{
	m_transformation->translate(x, y, z);
}

void Object3D::rotateX(float x)
{
	m_transformation->rotateX(x);
}
void Object3D::rotateY(float y)
{
	m_transformation->rotateY(y);
}
void Object3D::rotateZ(float z)
{
	m_transformation->rotateZ(z);
}

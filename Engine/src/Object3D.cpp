#include "Object3D.h"

#include "Transform.h"

Transformation& Object3D::getTransformation() const
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
	m_transformation->rotateLocalX(x);
}
void Object3D::rotateY(float y)
{
	m_transformation->rotateLocalY(y);
}
void Object3D::rotateZ(float z)
{
	m_transformation->rotateLocalZ(z);
}

#include "Object3D.h"

#include "Transform.h"

std::shared_ptr<Transform> Object3D::GetTransformation() const
{ 
	return m_transformation; 
}

uint32_t Object3D::getID() const
{ 
	return m_id; 
}

void Object3D::translate(float x, float y, float z)
{
	m_transformation->translate(x, y, z);
}

void Object3D::rotate(float x, float y, float z)
{
	//m_transformation->SetRotation();
}

void Object3D::rotateX(float x)
{
	rotate(x, 0, 0);
}
void Object3D::rotateY(float y)
{
	rotate(0, y, 0);
}
void Object3D::rotateZ(float z)
{
	rotate(0, 0, z);
}

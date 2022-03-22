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

void Object3D::rotateX(float x)
{
	static float m_rotationX = 0;
	m_rotationX += x;
	m_transformation->SetRotation(m_rotationX, { 1,0,0 });
}
void Object3D::rotateY(float y)
{
	static float m_rotationY = 0;
	m_rotationY += y;
	m_transformation->SetRotation(m_rotationY, { 0, 1,0 });
}
void Object3D::rotateZ(float z)
{
	static float m_rotationZ = 0;
	m_rotationZ += z;
	m_transformation->SetRotation(m_rotationZ, { 0, 0, 1 });
}

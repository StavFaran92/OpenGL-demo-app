#include "Object3D.h"

std::shared_ptr<Transform> Object3D::GetTransformation() const
{ 
	return m_transformation; 
}

uint32_t Object3D::getID() const
{ 
	return m_id; 
}

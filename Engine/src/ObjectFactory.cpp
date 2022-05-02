#include "ObjectFactory.h"

ObjectManager* ObjectFactory::m_objectManager = nullptr;

void ObjectFactory::init(ObjectManager* objectManager)
{
	m_objectManager = objectManager;
}

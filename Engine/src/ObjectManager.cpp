#include "ObjectManager.h"

#include "Object3D.h"

#include "Logger.h"

Object3D* ObjectManager::getObjectById(uint32_t id) const
{
    if (id > m_objectCounter)
    {
        logError("Illegal index specified: " + std::to_string(id));
        return nullptr;
    }

    if (m_objects.at(id) == nullptr)
    {
        logError("Object " + std::to_string(id) + " no longer avaiable.");
        return nullptr;
    }

    return m_objects.at(id).get();
}

void ObjectManager::addObject(Object3D* obj)
{
    obj->m_id = m_objectCounter;

    m_objects.insert({m_objectCounter, std::shared_ptr<Object3D>(obj)});

    m_objectCounter++;
}
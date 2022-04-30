#pragma once

#include <memory>
#include <unordered_map>

#include "Core.h"
#include "Object3D.h"

class EngineAPI ObjectManager
{
public:
	Object3D* getObjectById(uint32_t id) const;

private:
	friend class Object3D;

	void addObject(Object3D* obj);
private:
	uint32_t m_objectCounter = 0;

	std::unordered_map<uint32_t, std::shared_ptr<Object3D>> m_objects;
};


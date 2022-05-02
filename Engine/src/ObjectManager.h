#pragma once

#include <memory>
#include <unordered_map>

#include "Core.h"
#include "Object3D.h"

class ObjectFactory;

class EngineAPI ObjectManager
{
public:
	std::shared_ptr<Object3D> getObjectById(uint32_t id) const;

private:
	friend class ObjectFactory;

	void addObject(const std::shared_ptr<Object3D>& obj);
private:
	uint32_t m_objectCounter = 0;

	std::unordered_map<uint32_t, std::weak_ptr<Object3D>> m_objects;
};


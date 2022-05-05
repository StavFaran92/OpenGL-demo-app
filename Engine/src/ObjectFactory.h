#pragma once

#include <memory>

#include "Core.h"
#include "ObjectManager.h"
#include "Object3D.h"
#include "ObjectHandler.h"

class EngineAPI ObjectFactory
{
public:
	ObjectFactory() = delete;
	~ObjectFactory() = delete;

	static void init(ObjectManager* objectManager);

	template<typename T, typename... _Types>
	static ObjectHandler<T> create(_Types&&... _Args)
	{
		static_assert(std::is_base_of<Object3D, T>::value, "T must inherit from Object3D");

		auto obj = new T(std::forward<_Types>(_Args)...);

		uint32_t id = m_objectManager->addObject(obj);

		return ObjectHandler<T>(m_objectManager, id);
	}

private:
	static ObjectManager* m_objectManager;
};
#pragma once

#include <memory>

#include "Core.h"
#include "ObjectManager.h"
#include "Object3D.h"

class EngineAPI ObjectFactory
{
public:
	ObjectFactory() = delete;
	~ObjectFactory() = delete;

	static void init(ObjectManager* objectManager);

	template<typename T, typename... _Types>
	static std::shared_ptr<T> create(_Types&&... _Args)
	{
		static_assert(std::is_base_of<Object3D, T>::value, "T must inherit from Object3D");

		//auto obj = std::make_shared<T>(std::forward<_Types>(_Args)...);
		auto obj = std::shared_ptr<T>(new T(std::forward<_Types>(_Args)...));

		m_objectManager->addObject(obj);

		return obj;
	}

private:
	static ObjectManager* m_objectManager;
};
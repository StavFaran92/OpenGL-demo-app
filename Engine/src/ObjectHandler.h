#pragma once

#include "ObjectManager.h"

class Object3D;

template<typename T>
class ObjectHandler
{
public:
	/** Constructor */
	ObjectHandler() = default;
	virtual ~ObjectHandler() = default;

	/** Copy Constructor */
	ObjectHandler(const ObjectHandler& other);

	/** Copy Assignment Operator */
	ObjectHandler& operator=(const ObjectHandler& other);

	template <typename T2, typename std::enable_if_t<std::is_base_of_v<T2, T> || std::is_base_of_v<T, T2>, int> = 0>
	ObjectHandler(const ObjectHandler<T2>& other) 
		: m_objManager(other.m_objManager), m_objID(other.m_objID)
	{}

	T* object() const;
	void release();
	bool isValid() const;
	unsigned int getID() const;

	static ObjectHandler<Object3D> EmptyHandler;

private:
	friend class ObjectFactory;
	template<typename K> friend class ObjectHandler;
	ObjectHandler(ObjectManager* objManager, unsigned int modelID);
private:
	unsigned int m_objID = 0;
	ObjectManager* m_objManager = nullptr;
};

template<typename T>
ObjectHandler<Object3D> ObjectHandler<T>::EmptyHandler = ObjectHandler<Object3D>();

template<typename T>
inline ObjectHandler<T>::ObjectHandler(const ObjectHandler& other)
	: m_objManager(other.m_objManager), m_objID(other.m_objID)
{
}

template<typename T>
inline ObjectHandler<T>& ObjectHandler<T>::operator=(const ObjectHandler& other)
{
	m_objManager = other.m_objManager;
	m_objID = other.m_objID;

	return *this;
}

template<typename T>
T* ObjectHandler<T>::object() const
{
	return dynamic_cast<T*>(m_objManager->getObjectById(m_objID));
}

template<typename T>
void ObjectHandler<T>::release()
{
	m_objManager->releaseObject(m_objID);
}

template<typename T>
inline bool ObjectHandler<T>::isValid() const
{
	return m_objID != 0;
}

template<typename T>
inline unsigned int ObjectHandler<T>::getID() const
{
	return m_objID;
}

template<typename T>
ObjectHandler<T>::ObjectHandler(ObjectManager* objManager, unsigned int objID)
	:m_objManager(objManager), m_objID(objID)
{
}

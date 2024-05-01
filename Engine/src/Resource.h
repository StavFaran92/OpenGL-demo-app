#pragma once

#include <cstdint>

class Texture;
class Mesh;
#include "Engine.h"
#include "MemoryPool.h"

template<typename T>
class Resource
{
public:
	static Resource<T> empty;

	Resource() = default;

	Resource(std::nullptr_t) {};

	// Override assignment operator to allow assigning nullptr
	Resource& operator=(std::nullptr_t) {
		return empty;
	}

	Resource(const std::string& uid) : m_uid(uid) {};

	inline T* get() const
	{
		return Engine::get()->getMemoryPool<T>()->get(m_uid);
	}

	inline const std::string& getUID() const 
	{ 
		return m_uid; 
	}
private:
	std::string m_uid;
};
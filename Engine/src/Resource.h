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
		return nullptr;
	}

	inline const std::string& getUID() const 
	{ 
		return m_uid; 
	}
private:
	std::string m_uid;
};

// Specialization for get() function for Texture type
template<>
inline Texture* Resource<Texture>::get() const
{
	return Engine::get()->getMemoryPool<Texture>()->get(m_uid);
};

// Specialization for get() function for Texture type
template<>
inline Mesh* Resource<Mesh>::get() const
{
	return Engine::get()->getMemoryPool<Mesh>()->get(m_uid);
};
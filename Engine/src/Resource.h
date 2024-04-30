#pragma once

#include <cstdint>

#include "Texture.h"
#include "Engine.h"
#include "MemoryManagement.h"

template<typename T>
class Resource
{
public:
	Resource() = default;

	Resource(const std::string& uid) : m_uid(uid) {};

	inline T* get() const
	{
		return m_texturePool.get(m_uid);
	}

	inline const std::string& getUID() const 
	{ 
		return m_uid; 
	}
private:
	std::string m_uid = 0;
};

// Specialization for get() function for Texture type
template<>
inline Texture* Resource<Texture>::get() const
{
	return Engine::get()->getMemoryManagementSystem()->m_texturePool.get(m_uid);
}
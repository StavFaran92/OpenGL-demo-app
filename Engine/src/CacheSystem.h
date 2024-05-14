#pragma once

#include <memory>
#include <string>
#include <functional>
template<class T>  class Resource;

#include "MemoryPool.h"

class Texture;
class Mesh;
class Shader;

/**
 * This class is reponsible on holding all the shared heavy resources (Textures, Shaders, etc..) in memory.
 * It is used to optimize the system by caching the resources in memory until they are no longer needed.
 */
class CacheSystem
{
public:
	CacheSystem() = default;
	~CacheSystem() = default;

	template<typename T>
	Resource<T> createOrGetCached(const std::string& resourceName, const std::function<Resource<T>()>& creationCallback)
	{
		auto it = m_associations.find(resourceName);
		if (it != m_associations.end())
		{
			UUID uid = it->second;
			return Resource<T>(uid);
		}

		Resource<T>& resource = creationCallback();
		m_associations[resourceName] = resource.getUID();

		return resource;
	}

	void clear();
private:
	std::unordered_map<std::string, UUID> m_associations;
};


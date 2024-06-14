#pragma once

#include <memory>
#include <string>
#include <functional>
template<class T>  class Resource;

#include "MemoryPool.h"
#include "ProjectAssetRegistry.h"
#include "Context.h"
#include "Engine.h"
#include "ResourceManager.h"

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
	CacheSystem(const std::unordered_map<std::string, UUID>& associations);
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

		Engine::get()->getContext()->getProjectAssetRegistry()->addAssociation(resourceName, resource.getUID());

		return resource;
	}

	void clear();
private:
	std::unordered_map<std::string, UUID> m_associations;
};


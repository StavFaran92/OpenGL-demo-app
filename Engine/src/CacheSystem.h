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
	CacheSystem();
	~CacheSystem();
	Resource<Texture> getTexture(const std::string& resourceName, std::function<Texture* ()> creationCallback = nullptr);
	Resource<Mesh> getMesh(const std::string& resourceName, std::function<Resource<Mesh>()> creationCallback = nullptr);
	Resource<Shader> getShader(const std::string& resourceName, std::function<Shader* ()> creationCallback = nullptr);

	template<typename T>
	Resource<T> createOrGetCached(const std::string& resourceName, const std::function<Resource<T>()>& creationCallback)
	{
		auto it = m_associations.find(resourceName);
		if (it != m_associations.end())
		{
			auto& uid = it->second;
			Resource<T> resource(uid);
			return resource;
		}

		Resource<T>& resource = create(creationCallback);
		m_associations[resourceName] = resource.getUID();

		return resource;
	}
private:
	template<class T>friend class Resource;
	template<class T>friend class Factory;
	MemoryPool<Texture> m_texturePool;
	MemoryPool<Mesh> m_meshPool;
	MemoryPool<Shader> m_shaderPool;
	struct MemoryManagementImpl;
	MemoryManagementImpl* m_pimpl = nullptr;

	std::unordered_map<std::string, std::string> m_associations;
};


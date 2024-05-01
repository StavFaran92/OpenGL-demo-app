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
class MemoryManagement
{
public:
	MemoryManagement();
	~MemoryManagement();
	Resource<Texture> getTexture(const std::string& resourceName, std::function<Texture* ()> creationCallback = nullptr);
	Resource<Mesh> getMesh(const std::string& resourceName, std::function<Mesh* ()> creationCallback = nullptr);
	Resource<Shader> getShader(const std::string& resourceName, std::function<Shader* ()> creationCallback = nullptr);

private:
	template<class T>friend class Resource;
	template<class T>friend class Factory;
	MemoryPool<Texture> m_texturePool;
	MemoryPool<Mesh> m_meshPool;
	MemoryPool<Shader> m_shaderPool;
	struct MemoryManagementImpl;
	MemoryManagementImpl* m_pimpl = nullptr;
};


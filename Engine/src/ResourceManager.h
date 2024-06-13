#pragma once

#include <string>
#include <unordered_map>

#include "Resource.h"

class Texture;
class Mesh;

class ResourceManager
{
public:
	std::string getRootDir() const;
	void setRootDir(const std::string& rootDir);

    template<typename T> Resource<T> getResource(UUID uuid) const {};
    template<> Resource<Mesh> getResource(UUID uuid) const
    {
        auto iter = m_meshResources.find(uuid);
        if (iter != m_meshResources.end())
        {
            return iter->second;
        }
        return Resource<Mesh>::empty;
    }

    template<> Resource<Texture> getResource(UUID uuid) const
    {
        auto iter = m_textureResources.find(uuid);
        if (iter != m_textureResources.end())
        {
            return iter->second;
        }
        return Resource<Texture>::empty;
    }

    void addResource(const Resource<Mesh>& resource)
    {
        m_meshResources[resource.getUID()] = resource;
    }

    void addResource(const Resource<Texture>& resource)
    {
        m_textureResources[resource.getUID()] = resource;
    }

private:
	std::string m_rootResourceDir;

    std::unordered_map<UUID, Resource<Mesh>> m_meshResources;
    std::unordered_map<UUID, Resource<Texture>> m_textureResources;
};
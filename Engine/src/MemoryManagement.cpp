#include "MemoryManagement.h"



bool MemoryManagement::isTextureInCache(const std::string& filename) const
{
	return (m_texturesCache.find(filename) != m_texturesCache.end() && !m_texturesCache.at(filename).expired());
}

std::shared_ptr<Texture> MemoryManagement::getTextureFromCache(const std::string& filename) const
{
	return m_texturesCache.at(filename).lock();
}

void MemoryManagement::addTextureToCache(const std::string& filename, std::shared_ptr<Texture> texture)
{
	m_texturesCache[filename] = texture;
}

std::shared_ptr<Mesh> MemoryManagement::getMesh(const std::string& meshName)
{
    std::shared_ptr<Mesh> ptr;
    auto it = m_meshCache.find(meshName);
    if (it != m_meshCache.end())
    {
        ptr = it->second.lock();
        if (!ptr)
        {
            m_meshCache.erase(it);
        }
    }

    // If no mesh is found or the existing one was deleted, return nullptr.
    return ptr;
}

std::shared_ptr<Mesh> MemoryManagement::addMesh(const std::string& meshName, Mesh* mesh)
{
    auto meshPtr = std::shared_ptr<Mesh>(mesh);
	m_meshCache[meshName] = meshPtr;
    return meshPtr;
}

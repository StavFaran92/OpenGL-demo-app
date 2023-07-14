#include "MemoryManagement.h"

#include "MemoryPool.h"

struct MemoryManagement::MemoryManagementImpl
{
    MemoryPool<Texture> m_texturePool;
    MemoryPool<Mesh> m_meshPool;
};

std::shared_ptr<Texture> MemoryManagement::getTexture(const std::string& resourceName, std::function<Texture* ()> creationCallback)
{
	return m_pimpl->m_texturePool.get(resourceName, creationCallback);
}

std::shared_ptr<Mesh> MemoryManagement::getMesh(const std::string& resourceName, std::function<Mesh* ()> creationCallback)
{
	return m_pimpl->m_meshPool.get(resourceName, creationCallback);
}


MemoryManagement::MemoryManagement()
	: m_pimpl(new MemoryManagement::MemoryManagementImpl())
{
}

MemoryManagement::~MemoryManagement()
{
	delete m_pimpl;
}
#include "MemoryManagement.h"

#include "MemoryPool.h"

struct MemoryManagement::MemoryManagementImpl
{
    MemoryPool<Texture> m_texturePool;
    MemoryPool<Mesh> m_meshPool;
    MemoryPool<Shader> m_shaderPool;
};

std::shared_ptr<Texture> MemoryManagement::getTexture(const std::string& resourceName, std::function<Texture* ()> creationCallback/* = nullptr*/)
{
	return m_pimpl->m_texturePool.get(resourceName, creationCallback);
}

std::shared_ptr<Mesh> MemoryManagement::getMesh(const std::string& resourceName, std::function<Mesh* ()> creationCallback/* = nullptr*/)
{
	return m_pimpl->m_meshPool.get(resourceName, creationCallback);
}

std::shared_ptr<Shader> MemoryManagement::getShader(const std::string& resourceName, std::function<Shader* ()> creationCallback/* = nullptr*/)
{
	return m_pimpl->m_shaderPool.get(resourceName, creationCallback);
}


MemoryManagement::MemoryManagement()
	: m_pimpl(new MemoryManagement::MemoryManagementImpl())
{
}

MemoryManagement::~MemoryManagement()
{
	delete m_pimpl;
}
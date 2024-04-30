#include "MemoryManagement.h"

#include "Resource.h"

struct MemoryManagement::MemoryManagementImpl
{

};

Resource<Texture> MemoryManagement::getTexture(const std::string& resourceName, std::function<Texture* ()> creationCallback/* = nullptr*/)
{
	return m_texturePool.createOrGetCached(resourceName, creationCallback);
}

Resource<Mesh> MemoryManagement::getMesh(const std::string& resourceName, std::function<Mesh* ()> creationCallback/* = nullptr*/)
{
	return m_meshPool.createOrGetCached(resourceName, creationCallback);
}

Resource<Shader> MemoryManagement::getShader(const std::string& resourceName, std::function<Shader* ()> creationCallback/* = nullptr*/)
{
	return m_shaderPool.createOrGetCached(resourceName, creationCallback);
}


MemoryManagement::MemoryManagement()
	: m_pimpl(new MemoryManagement::MemoryManagementImpl())
{
}

MemoryManagement::~MemoryManagement()
{
	delete m_pimpl;
}
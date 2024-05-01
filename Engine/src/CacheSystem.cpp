#include "CacheSystem.h"

#include "Resource.h"

struct CacheSystem::MemoryManagementImpl
{

};

Resource<Texture> CacheSystem::getTexture(const std::string& resourceName, std::function<Texture* ()> creationCallback/* = nullptr*/)
{
	//return m_texturePool.createOrGetCached(resourceName, creationCallback); //todo fix
	return nullptr;
}

Resource<Mesh> CacheSystem::getMesh(const std::string& resourceName, std::function<Resource<Mesh> ()> creationCallback/* = nullptr*/)
{
	// search resource in association
		// if found get uuid 
			// go to memory pool using obtained uuid
		// if not found call callback
		// add pair to association
	// return resource

	auto iter = m_associations.find(resourceName);
	if (iter != m_associations.end())
	{
		UUID uid = iter->second;
		return Resource<Mesh>(uid);
	}
	else
	{
		Resource<Mesh> mesh = creationCallback();
		//todo check not null
		m_associations[resourceName] = mesh.getUID();
		return mesh;
	}
}

Resource<Shader> CacheSystem::getShader(const std::string& resourceName, std::function<Shader* ()> creationCallback/* = nullptr*/)
{
	//return m_shaderPool.createOrGetCached(resourceName, creationCallback);
	return nullptr;
}


CacheSystem::CacheSystem()
	: m_pimpl(new CacheSystem::MemoryManagementImpl())
{
}

CacheSystem::~CacheSystem()
{
	delete m_pimpl;
}
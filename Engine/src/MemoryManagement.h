#pragma once

#include <memory>
#include <string>
#include <functional>

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
	std::shared_ptr<Texture> getTexture(const std::string& resourceName, std::function<Texture* ()> creationCallback = nullptr);
	std::shared_ptr<Mesh> getMesh(const std::string& resourceName, std::function<Mesh* ()> creationCallback = nullptr);
	std::shared_ptr<Shader> getShader(const std::string& resourceName, std::function<Shader* ()> creationCallback = nullptr);

private:
	struct MemoryManagementImpl;
	MemoryManagementImpl* m_pimpl = nullptr;
};


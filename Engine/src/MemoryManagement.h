#pragma once

#include <unordered_map>
#include <memory>
#include <function>

class Texture;
class Mesh;

/**
 * This class is reponsible on holding all the shared heavy resources (Textures, Shaders, etc..) in memory.
 * It is used to optimize the system by caching the resources in memory until they are no longer needed.
 */
class MemoryManagement
{
public:
	bool isTextureInCache(const std::string& filename) const;
	std::shared_ptr<Texture> getTextureFromCache(const std::string& filename) const;
	void addTextureToCache(const std::string& filename, std::shared_ptr<Texture> texture);

	std::shared_ptr<Mesh> getMesh(const std::string& meshName);
	std::shared_ptr<Mesh> addMesh(const std::string& meshName, Mesh* mesh);

	//template<typename T>
	//std::shared_ptr<T> get(const std::string& resourceName, std::function<)

private:
	std::unordered_map<std::string, std::weak_ptr<Texture>> m_texturesCache;
	std::unordered_map<std::string, std::weak_ptr<Mesh>> m_meshCache;
};

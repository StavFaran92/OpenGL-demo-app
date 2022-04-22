#pragma once

#include <unordered_map>
#include <memory>

class Texture;

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

private:
	std::unordered_map<std::string, std::weak_ptr<Texture>> m_texturesCache;
};

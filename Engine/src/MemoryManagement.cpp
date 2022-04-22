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

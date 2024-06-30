#pragma once

#include "Engine.h"
#include "Resource.h"
#include "Texture.h"
#include "Core.h"

class EngineAPI Assets
{
public:
	Assets();

	Resource<Texture> importTexture2D(const std::string& path, bool flip = false);
	Resource<Texture> loadTexture2D(UUID uid, const std::string& path);
	std::vector<std::string> getAllTextures() const;

private:
	Texture::TextureData extractTextureDataFromFile(const std::string& fileLocation);

private:
	std::unordered_map<UUID, Resource<Texture>> m_textures;
};
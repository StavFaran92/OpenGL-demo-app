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

	static Texture::TextureData extractTextureDataFromFile(const std::string& fileLocation);
private:
};
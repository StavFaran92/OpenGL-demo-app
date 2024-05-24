#pragma once

#include <map>
#include "Resource.h"

class Texture;

class CommonTextures
{
public:
	enum class TextureType
	{
		BLACK_1X1,
		WHITE_1X1
	};

	CommonTextures();
	void close();
	Resource<Texture> getTexture(TextureType texType);

	CommonTextures(const CommonTextures&) = delete;
	CommonTextures& operator=(const CommonTextures&) = delete;
private:
	std::map<TextureType, Resource<Texture>> m_textures;
};
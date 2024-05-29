#include "CommonTextures.h"

#include "Texture.h"

CommonTextures::CommonTextures()
{
	unsigned char whiteColor[3] = {255, 255, 255};
	m_textures[TextureType::WHITE_1X1] = Texture::createDummyTexture(whiteColor);

	unsigned char blackColor[3] = { 0, 0, 0 };
	m_textures[TextureType::BLACK_1X1] = Texture::createDummyTexture(blackColor);
}

void CommonTextures::close()
{
	m_textures.clear();
}

Resource<Texture> CommonTextures::getTexture(TextureType texType)
{
	return m_textures[texType];
}

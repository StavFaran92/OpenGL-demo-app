#include "CommonTextures.h"

#include "Texture.h"

CommonTextures::CommonTextures()
{
	m_textures[TextureType::WHITE_1X1] = Texture::createDummyTexture();
}

void CommonTextures::close()
{
	m_textures.clear();
}

Resource<Texture> CommonTextures::getTexture(TextureType texType)
{
	return m_textures[texType];
}

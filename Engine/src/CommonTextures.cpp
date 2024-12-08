#include "CommonTextures.h"

#include "Assets.h"

CommonTextures::CommonTextures()
{
	Engine::get()->getSubSystem<Assets>()->importTexture2D("SGE_TEXTURE_WHITE", [this]() {
		unsigned char whiteColor[3] = { 255, 255, 255 };
		m_textures[TextureType::WHITE_1X1] = Texture::createDummyTexture(whiteColor);
		return m_textures[TextureType::WHITE_1X1];
		});

	

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

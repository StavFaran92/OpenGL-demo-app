#include "CommonTextures.h"

#include "Assets.h"
#include "Context.h"
#include "ProjectAssetRegistry.h"

#include "stb_image_write.h"

CommonTextures::CommonTextures()
{
}

void CommonTextures::close()
{
	m_textures.clear();
}

Resource<Texture> CommonTextures::getTexture(TextureType texType)
{
	return m_textures[texType];
}

CommonTextures* CommonTextures::create()
{
	auto instance = new CommonTextures();

	Engine::get()->getSubSystem<Assets>()->importTexture2D("SGE_TEXTURE_WHITE", [&]() {
		unsigned char whiteColor[3] = { 255, 255, 255 };
		auto texture = Texture::createDummyTexture(whiteColor);
		
		auto& projectDir = Engine::get()->getProjectDirectory();
		stbi_write_png((projectDir + "/" + texture.getUID() + ".png").c_str(), 1, 1, 3, whiteColor, 3);

		instance->m_textures[TextureType::WHITE_1X1] = texture;

		return instance->m_textures[TextureType::WHITE_1X1];
		});

	Engine::get()->getSubSystem<Assets>()->importTexture2D("SGE_TEXTURE_BLACK", [&]() {
		unsigned char blackColor[3] = { 0, 0, 0};
		auto texture = Texture::createDummyTexture(blackColor);

		auto& projectDir = Engine::get()->getProjectDirectory();
		stbi_write_png((projectDir + "/" + texture.getUID() + ".png").c_str(), 1, 1, 3, blackColor, 3);

		instance->m_textures[TextureType::BLACK_1X1] = texture;

		return instance->m_textures[TextureType::BLACK_1X1];
		});

	return instance;
}

CommonTextures* CommonTextures::load()
{
	auto instance = new CommonTextures();

	// TODO fix
	//Engine::get()->getContext()->getProjectAssetRegistry()->

	return instance;
}
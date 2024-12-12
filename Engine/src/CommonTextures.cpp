#include "CommonTextures.h"

#include "Assets.h"
#include "Context.h"
#include "ProjectAssetRegistry.h"
#include "CacheSystem.h"

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
		static unsigned char* whiteColor = new unsigned char[3]{ 255, 255, 255 }; // todo rethink this
		auto texture = Texture::createDummyTexture(whiteColor);
		instance->m_textures[TextureType::WHITE_1X1] = texture;
		return texture;
		});

	Engine::get()->getSubSystem<Assets>()->importTexture2D("SGE_TEXTURE_BLACK", [&]() {
		static unsigned char* blackColor = new unsigned char[3]{ 0, 0, 0};
		auto texture = Texture::createDummyTexture(blackColor);
		instance->m_textures[TextureType::BLACK_1X1] = texture;
		return texture;
		});

	return instance;
}

CommonTextures* CommonTextures::load()
{
	auto instance = new CommonTextures();

	instance->m_textures[TextureType::WHITE_1X1] = Engine::get()->getMemoryManagementSystem()->get<Texture>("SGE_TEXTURE_WHITE");
	instance->m_textures[TextureType::BLACK_1X1] = Engine::get()->getMemoryManagementSystem()->get<Texture>("SGE_TEXTURE_BLACK");

	// TODO fix
	//Engine::get()->getContext()->getProjectAssetRegistry()->

	return instance;
}
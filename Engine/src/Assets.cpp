#include "Assets.h"

#include <GL/glew.h>


#include "Texture.h"
#include "CacheSystem.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <filesystem>

Assets::Assets()
{
	Engine::get()->registerSubSystem<Assets>(this);
}

Texture::TextureData Assets::extractTextureDataFromFile(const std::string& fileLocation)
{
	Texture::TextureData textureData;
	textureData.target = GL_TEXTURE_2D;

	textureData.data = stbi_load(fileLocation.c_str(), &textureData.width, &textureData.height, &textureData.bpp, 0);

	// load validation
	if (!textureData.data)
	{
		logError("Failed to find: {}", fileLocation.c_str());
		return {};
	}

	GLenum format = GL_RGB;
	if (textureData.bpp == 1)
		textureData.format = GL_RED;
	else if (textureData.bpp == 3)
		textureData.format = GL_RGB;
	else if (textureData.bpp == 4)
		textureData.format = GL_RGBA;

	textureData.internalFormat = textureData.format;

	textureData.type = GL_UNSIGNED_BYTE;
	textureData.params = {
		{ GL_TEXTURE_WRAP_S, GL_REPEAT},
		{ GL_TEXTURE_WRAP_T, GL_REPEAT},
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR},
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR},
	};

	textureData.genMipMap = true;

	return textureData;
}

Resource<Texture> Assets::importTexture2D(const std::string& fileLocation, bool flip)
{
	// Check if texture is already cached to optimize the load process
	auto memoryManagementSystem = Engine::get()->getMemoryManagementSystem();
	std::filesystem::path path(fileLocation);
	return memoryManagementSystem->createOrGetCached<Texture>(path.filename().string(), [&]() {

		// todo use RAII
		stbi_set_flip_vertically_on_load(flip);
		Texture::TextureData textureData = extractTextureDataFromFile(fileLocation);

		Resource<Texture> texture = Texture::create2DTextureFromBuffer(textureData);

		auto& projectDir = Engine::get()->getProjectDirectory();
		stbi_write_png((projectDir + "/" + texture.getUID() + ".png").c_str(), textureData.width, textureData.height, textureData.bpp, textureData.data, textureData.width * textureData.bpp);
		Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(texture.getUID());

		m_textures[texture.getUID()] = texture;

		stbi_image_free(textureData.data);

		return texture;
		});
}

Resource<Texture> Assets::loadTexture2D(UUID uid, const std::string& path)
{
	Texture::TextureData textureData = extractTextureDataFromFile(path);

	// Create texture
	Texture* texture = new Texture();
	texture->build(textureData);
	Engine::get()->getMemoryPool<Texture>()->add(uid, texture);

	auto& res = Resource<Texture>(uid);

	m_textures[uid] = res;

	return res;
}

std::vector<std::string> Assets::getAllTextures() const
{
	std::vector<std::string> result;
	for (auto [uuid, _] : m_textures)
	{
		result.push_back(uuid);
	}
	return result;
}

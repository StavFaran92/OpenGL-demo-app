#include "Cubemap.h"
#include "ApplicationConstants.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Logger.h"
#include "Configurations.h"
#include "CacheSystem.h"
#include "Engine.h"
#include "Resource.h"
#include "Assets.h"
#include "Factory.h"
#include "Context.h"
#include "ProjectAssetRegistry.h"

#include "Texture.h" 

#include "EquirectangularToCubemapConverter.h"

Resource<Texture> Cubemap::createCubemapFromCubemapFiles(const std::vector<std::string>& faces)
{
	// Check if texture is already cached to optimize the load process
	auto memoryManagementSystem = Engine::get()->getMemoryManagementSystem();
	std::filesystem::path path(faces[0]);
	return memoryManagementSystem->createOrGetCached<Texture>(path.filename().string(), [&]() {

		// todo use RAII
		CubemapData cubemapData = extractCubemapDataFromCubemapFiles(faces);

		Resource<Texture> cubemap = createCubemapFromBuffer(cubemapData);

		//Resource<Texture> equirectangularMap = EquirectangularToCubemapConverter::fromCubemapToEquirectangular(cubemap);

		//equirectangularMap.get()->bind();

		//// Allocate memory for the pixels
		//void* pixels = malloc(equirectangularMap.get()->getWidth() * equirectangularMap.get()->getHeight() * equirectangularMap.get()->getBitDepth());

		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		//auto& projectDir = Engine::get()->getProjectDirectory();
		//stbi_write_png((projectDir + "/" + cubemap.getUID() + ".png").c_str(), equirectangularMap.get()->getWidth(), equirectangularMap.get()->getHeight(), equirectangularMap.get()->getBitDepth(), pixels, 
		//	equirectangularMap.get()->getWidth() * equirectangularMap.get()->getBitDepth());
		//Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(cubemap.getUID());

		//free(cubemapData.data);

		return cubemap;
	});
}

Cubemap::CubemapData Cubemap::extractCubemapDataFromEquirectangularFile(const std::string& fileLocation)
{


	auto equirectangularMap = Engine::get()->getSubSystem<Assets>()->importTexture2D(fileLocation);
	

	CubemapData cubemapData;
	cubemapData.target = GL_TEXTURE_CUBE_MAP;

	int width, height, nrChannels;
	cubemapData.data[0] = stbi_load(fileLocation.c_str(), &cubemapData.width, &cubemapData.height, &cubemapData.bpp, 0);

	cubemapData.format = GL_RGB;
	cubemapData.internalFormat = GL_RGB;

	cubemapData.type = GL_UNSIGNED_BYTE;
	cubemapData.params = {
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR},
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		{ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
		{ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
		{ GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE},
	};

	cubemapData.genMipMap = false;

	return cubemapData;
	//return {};
}

Resource<Texture> Cubemap::createCubemapFromEquirectangularFile(const std::string& fileLocation)
{
	// Check if texture is already cached to optimize the load process
	auto memoryManagementSystem = Engine::get()->getMemoryManagementSystem();
	std::filesystem::path path(fileLocation);
	return memoryManagementSystem->createOrGetCached<Texture>(path.filename().string(), [&]() {

		//open equirect file
		//create equirect texture
		auto equirectangularMap = Engine::get()->getSubSystem<Assets>()->importTexture2D(fileLocation);
		//convert equirect to cubemap
		auto cubemap = EquirectangularToCubemapConverter::fromEquirectangularToCubemap(equirectangularMap);

		// todo use RAII
		//CubemapData cubemapData = extractCubemapDataFromEquirectangularFile(fileLocation);

		//Resource<Texture> cubemap = createCubemapFromBuffer(cubemapData);

		equirectangularMap.get()->bind();

		// Allocate memory for the pixels
		void* pixels = malloc(equirectangularMap.get()->getWidth() * equirectangularMap.get()->getHeight() * equirectangularMap.get()->getBitDepth());

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		auto& projectDir = Engine::get()->getProjectDirectory();
		stbi_write_png((projectDir + "/" + cubemap.getUID() + ".png").c_str(), equirectangularMap.get()->getWidth(), equirectangularMap.get()->getHeight(), equirectangularMap.get()->getBitDepth(), pixels,
			equirectangularMap.get()->getWidth() * equirectangularMap.get()->getBitDepth());
		Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(cubemap.getUID());

		free(pixels);

		return cubemap;
	});
}

Resource<Texture> Cubemap::createCubemapFromBuffer(const CubemapData& cubemapData)
{
	return build(cubemapData);
}

Resource<Texture> Cubemap::createEmptyCubemap(int width, int height, int internalFormat, int format, int type)
{
	CubemapData cubemapData;
	cubemapData.target = GL_TEXTURE_CUBE_MAP;
	cubemapData.width = width;
	cubemapData.height = height;
	cubemapData.internalFormat = internalFormat;
	cubemapData.format = format;
	cubemapData.type = type;
	cubemapData.params = {
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR},
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		{ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
		{ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
		{ GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE},
	};
	cubemapData.genMipMap = false;

	return createCubemapFromBuffer(cubemapData);
}

Resource<Texture> Cubemap::createEmptyCubemap(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, bool createMipMaps)
{
	CubemapData cubemapData;
	cubemapData.target = GL_TEXTURE_CUBE_MAP;
	cubemapData.width = width;
	cubemapData.height = height;
	cubemapData.internalFormat = internalFormat;
	cubemapData.format = format;
	cubemapData.type = type;
	cubemapData.params = params;
	cubemapData.genMipMap = createMipMaps;

	return createCubemapFromBuffer(cubemapData);
}

Cubemap::CubemapData Cubemap::extractCubemapDataFromCubemapFiles(const std::vector<std::string>& files)
{
	CubemapData cubemapData;
	cubemapData.target = GL_TEXTURE_CUBE_MAP;

	int width, height, nrChannels;
	for (unsigned int i = 0; i < files.size(); i++)
	{
		cubemapData.data[i] = stbi_load(files[i].c_str(), &cubemapData.width, &cubemapData.height, &cubemapData.bpp, 0);
	}

	cubemapData.format = GL_RGB;
	cubemapData.internalFormat = GL_RGB;

	cubemapData.type = GL_UNSIGNED_BYTE;
	cubemapData.params = {
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR},
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		{ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
		{ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
		{ GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE},
	};

	cubemapData.genMipMap = false;

	return cubemapData;
}

Resource<Texture> Cubemap::build(const CubemapData& textureData)
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->m_target = textureData.target;
	texture.get()->m_width = textureData.width;
	texture.get()->m_height = textureData.height;

	// generate texture
	glGenTextures(1, &texture.get()->m_id);
	texture.get()->bind();

	for (auto& [paramKey, paramValue] : textureData.params)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, paramKey, paramValue);
	}

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureData.internalFormat, texture.get()->m_width, texture.get()->m_height, 0, textureData.format, textureData.type, textureData.data[i]);
		stbi_image_free(textureData.data[i]);
	}
	if (textureData.genMipMap)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	texture.get()->unbind();

	return texture;
}

void Cubemap::saveEquirectangularMap(Resource<Texture> equirectangularMap)
{
	equirectangularMap.get()->bind();

	// Allocate memory for the pixels
	void* pixels = malloc(equirectangularMap.get()->getWidth() * equirectangularMap.get()->getHeight() * equirectangularMap.get()->getBitDepth());

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	auto& projectDir = Engine::get()->getProjectDirectory();
	stbi_write_png((projectDir + "/" + equirectangularMap.getUID() + ".png").c_str(), equirectangularMap.get()->getWidth(), equirectangularMap.get()->getHeight(), 3, pixels,
		equirectangularMap.get()->getWidth() * 3);
	Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(equirectangularMap.getUID());
}

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
#include "Factory.h"
#include "Context.h"
#include "ProjectAssetRegistry.h"

#include "EquirectangularToCubemapConverter.h" // todo remove

Resource<Cubemap> Cubemap::createCubemapFromFile(const std::vector<std::string>& faces)
{
	// Check if texture is already cached to optimize the load process
	auto memoryManagementSystem = Engine::get()->getMemoryManagementSystem();
	std::filesystem::path path(faces[0]);
	return memoryManagementSystem->createOrGetCached<Cubemap>(path.filename().string(), [&]() {

		// todo use RAII
		CubemapData cubemapData = extractCubemapDataFromMultipleFiles(faces);

		Resource<Cubemap> texture = createCubemapFromBuffer(cubemapData);

		auto& projectDir = Engine::get()->getProjectDirectory();
		stbi_write_png((projectDir + "/" + texture.getUID() + ".png").c_str(), cubemapData.width, cubemapData.height, cubemapData.bpp, cubemapData.data, cubemapData.width * cubemapData.bpp);
		Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(texture.getUID());

		free(cubemapData.data);

		return texture;
	});
}

Resource<Cubemap> Cubemap::createCubemapFromFile(const std::string& fileLocation)
{
	return Resource<Cubemap>();
}

Resource<Cubemap> Cubemap::createCubemapFromBuffer(const CubemapData& cubemapData)
{
	Resource<Cubemap> cubemap = Factory<Cubemap>::create();

	cubemap.get()->build(cubemapData);

	return cubemap;
}

Resource<Cubemap> Cubemap::createEmptyCubemap(int width, int height, int internalFormat, int format, int type)
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

Resource<Cubemap> Cubemap::createEmptyCubemap(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, bool createMipMaps)
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

Cubemap::CubemapData Cubemap::extractCubemapDataFromMultipleFiles(const std::vector<std::string>& files)
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

void Cubemap::build(const CubemapData& textureData)
{
	m_target = textureData.target;
	m_width = textureData.width;
	m_height = textureData.height;

	// generate texture
	glGenTextures(1, &m_id);
	bind();

	for (auto& [paramKey, paramValue] : textureData.params)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, paramKey, paramValue);
	}

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureData.internalFormat, m_width, m_height, 0, textureData.format, textureData.type, textureData.data[i]);
	}
	if (textureData.genMipMap)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	unbind();
}

void Cubemap::bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(m_target, m_id);
}

void Cubemap::unbind() const
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(m_target, 0);
}

void Cubemap::flip()
{
	m_flipped = !m_flipped;
}

bool Cubemap::isFlipped() const
{
	return m_flipped;
}

unsigned int Cubemap::getID() const
{
	return m_id;
}

void Cubemap::clear()
{
	glDeleteTextures(1, &m_id);
}

Cubemap::~Cubemap()
{
	logInfo( __FUNCTION__ );
	clear();
}
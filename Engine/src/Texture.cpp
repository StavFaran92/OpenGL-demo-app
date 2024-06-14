#include "Texture.h"
#include "ApplicationConstants.h"

#include <GL/glew.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
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

Texture::Texture()
	:m_id(0), m_width(0), m_height(0), m_bitDepth(0), m_slot(0)
{
}

Texture::Texture(const Texture& other)
	: m_id(other.m_id), m_width(other.m_width), m_height(other.m_height), m_bitDepth(other.m_bitDepth), m_slot(other.m_slot)
{
}

Resource<Texture> Texture::createEmptyTexture(int width, int height)
{
	return createEmptyTexture(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}

Resource<Texture> Texture::createEmptyTexture(int width, int height, int internalFormat, int format, int type)
{
	TextureData textureData;
	textureData.target = GL_TEXTURE_2D;
	textureData.width = width;
	textureData.height = height;
	textureData.internalFormat = internalFormat;
	textureData.format = format;
	textureData.type = type;
	textureData.params = {
		{GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		{GL_TEXTURE_MAG_FILTER, GL_NEAREST },
	};

	return create2DTextureFromBuffer(textureData);
}

Resource<Texture> Texture::create2DTextureFromFile(const std::string& fileLocation, bool flip/* = FLIP_TEXTURE*/)
{
	// Check if texture is already cached to optimize the load process
	auto memoryManagementSystem = Engine::get()->getMemoryManagementSystem();
	std::filesystem::path path(fileLocation);
	return memoryManagementSystem->createOrGetCached<Texture>(path.filename().string(), [&]() {
		
		// todo use RAII
		TextureData textureData = extractTextureDataFromFile(fileLocation);

		stbi_set_flip_vertically_on_load(flip);

		Resource<Texture> texture = create2DTextureFromBuffer(textureData);

		auto& projectDir = Engine::get()->getProjectDirectory();
		stbi_write_png((projectDir + "/" + texture.getUID() + ".png").c_str(), textureData.width, textureData.height, textureData.bpp, textureData.data, textureData.width * textureData.bpp);
		Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(texture.getUID());

		stbi_image_free(textureData.data);

		return texture;
	});
}

Resource<Texture> Texture::create2DTextureFromBuffer(const TextureData& textureData)
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->build(textureData);

	return texture;
}

Resource<Texture> Texture::create2DTextureFromBuffer(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, void* data)
{
	TextureData textureData;
	textureData.target = GL_TEXTURE_2D;
	textureData.width = width;
	textureData.height = height;
	textureData.internalFormat = internalFormat;
	textureData.format = format;
	textureData.type = type;
	textureData.params = params;
	textureData.data = data;

	return create2DTextureFromBuffer(textureData);
}

Resource<Texture> Texture::createDummyTexture(unsigned char data[3])
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->m_target = GL_TEXTURE_2D;
	texture.get()->m_width = 1;
	texture.get()->m_height = 1;

	// generate texture
	glGenTextures(1, &texture.get()->m_id);
	texture.get()->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	texture.get()->unbind();

	return texture;
}

Texture::TextureData Texture::extractTextureDataFromFile(const std::string& fileLocation)
{
	TextureData textureData;
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
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR},
	};

	textureData.genMipMap = true;

	return textureData;
}

void Texture::build(const TextureData& textureData)
{
	m_target = textureData.target;
	m_width = textureData.width;
	m_height = textureData.height;
	m_bitDepth = textureData.bpp;

	// generate texture
	glGenTextures(1, &m_id);
	bind();

	for (auto& [paramKey, paramValue] : textureData.params)
	{
		glTexParameteri(GL_TEXTURE_2D, paramKey, paramValue);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, textureData.internalFormat, m_width, m_height, 0, textureData.format, textureData.type, textureData.data);

	if (textureData.genMipMap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	unbind();
}

int Texture::getWidth() const
{
	return m_width;
}

int Texture::getHeight() const
{
	return m_height;
}

int Texture::getBitDepth() const
{
	return m_bitDepth;
}

std::string Texture::textureTypeToString(Type type)
{
	switch (type)
	{
		case Texture::Type::Diffuse:
			return Constants::g_textureDiffuse;
		case Texture::Type::Specular:
			return Constants::g_textureSpecular;
		case Texture::Type::Albedo:
			return Constants::g_textureAlbedo;
		case Texture::Type::Normal:
			return Constants::g_textureNormal;
		case Texture::Type::Metallic:
			return Constants::g_textureMetallic;
		case Texture::Type::Roughness:
			return Constants::g_textureRoughness;
		case Texture::Type::AmbientOcclusion:
			return Constants::g_textureAO;

		default:
			logError("Unsupported texture format");
			return "";
	}
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(m_target, m_id);
}

void Texture::unbind() const
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(m_target, 0);
}

void Texture::flip()
{
	m_flipped = !m_flipped;
}

bool Texture::isFlipped() const
{
	return m_flipped;
}

unsigned int Texture::getID() const
{
	return m_id;
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &m_id);
}

Texture::~Texture()
{
	logInfo( __FUNCTION__ );
	ClearTexture();
}
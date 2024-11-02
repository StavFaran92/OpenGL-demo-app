#include "Texture.h"
#include "ApplicationConstants.h"

#include <GL/glew.h>

#include "Logger.h"
#include "Configurations.h"
#include "CacheSystem.h"
#include "Engine.h"
#include "Resource.h"
#include "Factory.h"
#include "Context.h"

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
	textureData.bpp = 4;
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

void Texture::setData(int xoffset, int yoffset, int width, int height, const void* data)
{
	bind();

	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

std::string Texture::textureTypeToString(Type type)
{
	switch (type)
	{
		case Texture::Type::Diffuse:
			return Constants::g_textureAlbedo;
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
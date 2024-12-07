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
	:m_id(0), m_slot(0)
{
}

Texture::Texture(const Texture& other)
	: m_id(other.m_id), m_slot(other.m_slot), m_data(other.m_data)
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
	textureData.internalFormat = (InternalFormat)internalFormat;
	textureData.format = (Format)format;
	textureData.type = (Type)type;
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
	textureData.internalFormat = (InternalFormat)internalFormat;
	textureData.format = (Format)format;
	textureData.type = (Type)type;
	textureData.params = params;
	textureData.data = data;

	return create2DTextureFromBuffer(textureData);
}

Resource<Texture> Texture::createDummyTexture(unsigned char data[3])
{
	TextureData tData;
	tData.target = GL_TEXTURE_2D;
	tData.width = 1;
	tData.height = 1;
	tData.params = { {GL_TEXTURE_MIN_FILTER, GL_LINEAR},
					{GL_TEXTURE_MAG_FILTER, GL_LINEAR},
					{GL_TEXTURE_WRAP_S, GL_REPEAT},
					{GL_TEXTURE_WRAP_T, GL_REPEAT } };

	return create2DTextureFromBuffer(tData);
}

void Texture::build(const TextureData& textureData)
{
	m_data = textureData;

	// generate texture
	glGenTextures(1, &m_id);
	bind();

	for (auto& [paramKey, paramValue] : textureData.params)
	{
		glTexParameteri(GL_TEXTURE_2D, paramKey, paramValue);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, textureData.internalFormat, m_data.width, m_data.height, 0, textureData.format, (int)textureData.type, textureData.data);

	if (textureData.genMipMap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	unbind();
}

int Texture::getWidth() const
{
	return m_data.width;
}

int Texture::getHeight() const
{
	return m_data.height;
}

int Texture::getBitDepth() const
{
	return m_data.bpp;
}

void Texture::setData(int xoffset, int yoffset, int width, int height, const void* data)
{
	bind();

	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

std::string Texture::textureTypeToString(TextureType type)
{
	switch (type)
	{
		case Texture::TextureType::Diffuse:
			return Constants::g_textureAlbedo;
		case Texture::TextureType::Specular:
			return Constants::g_textureSpecular;
		case Texture::TextureType::Albedo:
			return Constants::g_textureAlbedo;
		case Texture::TextureType::Normal:
			return Constants::g_textureNormal;
		case Texture::TextureType::Metallic:
			return Constants::g_textureMetallic;
		case Texture::TextureType::Roughness:
			return Constants::g_textureRoughness;
		case Texture::TextureType::AmbientOcclusion:
			return Constants::g_textureAO;

		default:
			logError("Unsupported texture format");
			return "";
	}
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(m_data.target, m_id);
}

void Texture::unbind() const
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(m_data.target, 0);
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
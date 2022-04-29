#include "Texture.h"
#include "ApplicationConstants.h"

#include <GL/glew.h>
#include "stb_image.h"

#include "Logger.h"
#include "Configurations.h"
#include "MemoryManagement.h"
#include "Engine.h"
#include "TextureHandler.h"

Texture::Texture()
	:m_id(0), m_width(0), m_height(0), m_bitDepth(0), m_slot(0)
{
	logInfo( __FUNCTION__ );
}

Texture::Texture(const Texture& other)
	: m_id(other.m_id), m_width(other.m_width), m_height(other.m_height), m_bitDepth(other.m_bitDepth), m_slot(other.m_slot)
{
	logInfo(__FUNCTION__);
}

TextureHandler* Texture::createEmptyTexture(int width, int height)
{
	return createEmptyTexture(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}

TextureHandler* Texture::createEmptyTexture(int width, int height, int internalFormat, int format, int type)
{
	auto texture = new Texture();

	texture->m_target = GL_TEXTURE_2D;
	texture->m_width = width;
	texture->m_height = height;

	// generate texture
	glGenTextures(1, &texture->m_id);
	texture->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->unbind();

	auto textureHandler = new TextureHandler(texture);

	return textureHandler;
}

TextureHandler* Texture::loadTextureFromFile(const std::string& fileLocation)
{
	// Check if texture is already cached to optimize the load process
	auto memoryManagementSystem = Engine::get()->getMemoryManagementSystem();
	if (memoryManagementSystem->isTextureInCache(fileLocation))
	{
		return new TextureHandler(memoryManagementSystem->getTextureFromCache(fileLocation));
	}

	auto texture = new Texture();

	texture->m_target = GL_TEXTURE_2D;

	// flip the image
	stbi_set_flip_vertically_on_load(FLIP_TEXTURE);

	// load texture from file
	unsigned char* data = stbi_load(fileLocation.c_str(), &texture->m_width, &texture->m_height, &texture->m_bitDepth, 0);

	// load validation
	if (!data) {
		logError("Failed to find: {}", fileLocation.c_str());
		return nullptr;
	}

	// generate texture and bind it
	glGenTextures(1, &texture->m_id);
	texture->bind();

	GLenum format = GL_RGB;
	if (texture->m_bitDepth == 1)
		format = GL_RED;
	else if (texture->m_bitDepth == 3)
		format = GL_RGB;
	else if (texture->m_bitDepth == 4)
		format = GL_RGBA;

	// sets the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// generate texture and mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, format, texture->m_width, texture->m_height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// unbind texture and release the image.
	texture->unbind();
	stbi_image_free(data);

	// Cache texture
	auto texturePtr = std::shared_ptr<Texture>(texture);
	memoryManagementSystem->addTextureToCache(fileLocation, texturePtr);

	auto textureHandler = new TextureHandler(texturePtr);

	return textureHandler;
}

TextureHandler* Texture::loadCubemapTexture(std::vector<std::string> faces)
{
	auto texture = new Texture();

	texture->m_target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &texture->m_id);
	texture->bind();

	// flip the image
	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			logError("Cubemap tex failed to load at path: {}", faces[i]);
			stbi_image_free(data);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture->unbind();

	auto textureHandler = new TextureHandler(texture);

	return textureHandler;
}

std::string Texture::textureTypeToString(Type type)
{
	switch (type)
	{
		case Texture::Type::Diffuse:
			return Constants::g_textureDiffuse;
		case Texture::Type::Specular:
			return Constants::g_textureSpecular;
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
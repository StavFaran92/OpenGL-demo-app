#include "Texture.h"
#include "ApplicationConstants.h"

#include <GL/glew.h>
#include "stb_image.h"

#include "Logger.h"

Texture::Texture()
	:m_id(0), m_width(0), m_height(0), m_bitDepth(0), m_slot(0)
{
	logTrace( __FUNCTION__ );
}

std::shared_ptr<Texture> Texture::createEmptyTexture(int width, int height)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->setTarget(GL_TEXTURE_2D);
	// generate texture
	glGenTextures(1, &texture->m_id);
	glBindTexture(texture->getTarget(), texture->m_id);
	glTexImage2D(texture->getTarget(), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(texture->getTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(texture->getTarget(), 0);

	return texture;
}

std::shared_ptr<Texture> Texture::loadTextureFromFile(const std::string& fileLocation, bool isFlipped)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->setTarget(GL_TEXTURE_2D);

	// Cache file location
	texture->m_fileLocation = fileLocation;

	// flip the image
	stbi_set_flip_vertically_on_load(isFlipped);

	// load texture from file
	unsigned char* data = stbi_load(texture->m_fileLocation.c_str(), &texture->m_width, &texture->m_height, &texture->m_bitDepth, 0);

	// load validation
	if (!data) {
		logError("Failed to find: {}", texture->m_fileLocation.c_str());
		return nullptr;
	}

	// generate texture and bind it
	glGenTextures(1, &texture->m_id);
	glBindTexture(texture->getTarget(), texture->m_id);

	GLenum format = GL_RGB;
	if (texture->m_bitDepth == 1)
		format = GL_RED;
	else if (texture->m_bitDepth == 3)
		format = GL_RGB;
	else if (texture->m_bitDepth == 4)
		format = GL_RGBA;

	// sets the texture parameters
	glTexParameteri(texture->getTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// generate texture and mipmaps
	glTexImage2D(texture->getTarget(), 0, format, texture->m_width, texture->m_height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(texture->getTarget());

	// unbind texture and release the image.
	glBindTexture(texture->getTarget(), 0);
	stbi_image_free(data);

	return texture;
}

std::shared_ptr<Texture> Texture::loadCubemapTexture(std::vector<std::string> faces)
{
	auto texture = std::make_shared<Texture>();
	glGenTextures(1, &texture->m_id);

	texture->setTarget(GL_TEXTURE_CUBE_MAP);

	glBindTexture(texture->getTarget(), texture->m_id);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			logError("Cubemap tex failed to load at path: {}", faces[i]);
			stbi_image_free(data);
		}
	}
	glTexParameteri(texture->getTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(texture->getTarget(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
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

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(m_target, m_id);
}

void Texture::unbind()
{
	glActiveTexture(0);
}

void Texture::setType(Type type)
{
	m_type = type;
}

unsigned int Texture::getID() const
{
	return m_id;
}

Texture::Type Texture::getType() const
{
	return m_type;
}

std::string Texture::getFilepath() const
{
	return m_fileLocation;
}

uint32_t Texture::getTarget() const
{
	return m_target;
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &m_id);
}

Texture::~Texture()
{
	logTrace( __FUNCTION__ );
	ClearTexture();
}
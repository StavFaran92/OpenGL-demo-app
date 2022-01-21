#include "Texture.h"
#include "Vendor/stb_image/stb_image.h"

Texture::Texture()
	:m_id(0), m_width(0), m_height(0), m_bitDepth(0), m_slot(0)
{
	logTrace( __FUNCTION__ );
}

std::shared_ptr<Texture> Texture::CreateEmptyTexture(int width, int height)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	// generate texture
	glGenTextures(1, &texture->m_id);
	glBindTexture(GL_TEXTURE_2D, texture->m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

std::shared_ptr<Texture> Texture::LoadTextureFromFile(const std::string& fileLocation, bool isFlipped)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

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
	glBindTexture(GL_TEXTURE_2D, texture->m_id);

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
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return texture;
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
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
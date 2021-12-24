#include "Texture.h"
#include "Vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& fileLocation, std::string typeName, int slot)
	:m_id(0), m_width(0), m_height(0), m_bitDepth(0), m_fileLocation(fileLocation), m_slot(slot), m_type(typeName)
{

}

void Texture::LoadTexture(bool isFlipped)
{
	// flip the image
	stbi_set_flip_vertically_on_load(isFlipped);

	// load texture from file
	unsigned char *data = stbi_load(m_fileLocation.c_str(), &m_width, &m_height, &m_bitDepth, 0);

	// load validation
	if (!data) {
		fprintf(stderr, "Failed to find: %s\n", m_fileLocation.c_str());
		return;
	}

	// generate texture and bind it
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	GLenum format = GL_RGB;
	if (m_bitDepth == 1)
		format = GL_RED;
	else if (m_bitDepth == 3)
		format = GL_RGB;
	else if (m_bitDepth == 4)
		format = GL_RGBA;

	// sets the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// generate texture and mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// unbind texture and release the image.
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
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
	std::cout << __FUNCTION__ << std::endl;
	//ClearTexture();
}
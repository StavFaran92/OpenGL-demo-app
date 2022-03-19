#include "Texture.h"
#include "stb_image.h"

Texture::Texture()
	:m_id(0), m_width(0), m_height(0), m_bitDepth(0), m_slot(0)
{
	logTrace( __FUNCTION__ );
}

std::shared_ptr<Texture> Texture::CreateEmptyTexture(int width, int height)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->SetCategory(GL_TEXTURE_2D);
	// generate texture
	glGenTextures(1, &texture->m_id);
	glBindTexture(texture->GetCategory(), texture->m_id);
	glTexImage2D(texture->GetCategory(), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(texture->GetCategory(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(texture->GetCategory(), 0);

	return texture;
}

std::shared_ptr<Texture> Texture::LoadTextureFromFile(const std::string& fileLocation, bool isFlipped)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->SetCategory(GL_TEXTURE_2D);

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
	glBindTexture(texture->GetCategory(), texture->m_id);

	GLenum format = GL_RGB;
	if (texture->m_bitDepth == 1)
		format = GL_RED;
	else if (texture->m_bitDepth == 3)
		format = GL_RGB;
	else if (texture->m_bitDepth == 4)
		format = GL_RGBA;

	// sets the texture parameters
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// generate texture and mipmaps
	glTexImage2D(texture->GetCategory(), 0, format, texture->m_width, texture->m_height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(texture->GetCategory());

	// unbind texture and release the image.
	glBindTexture(texture->GetCategory(), 0);
	stbi_image_free(data);

	return texture;
}

std::shared_ptr<Texture> Texture::LoadCubemap(std::vector<std::string> faces)
{
	auto texture = std::make_shared<Texture>();
	glGenTextures(1, &texture->m_id);

	texture->SetCategory(GL_TEXTURE_CUBE_MAP);

	glBindTexture(texture->GetCategory(), texture->m_id);

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
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(texture->GetCategory(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(GetCategory(), m_id);
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
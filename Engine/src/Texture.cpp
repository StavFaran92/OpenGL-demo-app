#include "Texture.h"
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
	return create2DTextureFromBuffer(width, height, internalFormat, format, type, {
		{GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		{GL_TEXTURE_MAG_FILTER, GL_NEAREST },
		}, NULL);
}

Resource<Texture> Texture::create2DTextureFromFile(const std::string& fileLocation, bool flip/* = FLIP_TEXTURE*/)
{
	// Check if texture is already cached to optimize the load process
	auto memoryManagementSystem = Engine::get()->getMemoryManagementSystem();
	std::filesystem::path path(fileLocation);
	return memoryManagementSystem->createOrGetCached<Texture>(path.filename().string(), [&]() {
		

		int target = GL_TEXTURE_2D;
		int width = 0;
		int height = 0;
		int bpp = 0;

		// flip the image
		stbi_set_flip_vertically_on_load(flip);

		// load texture from file
		unsigned char* data = stbi_load(fileLocation.c_str(), &width, &height, &bpp, 0);

		// load validation
		if (!data) 
		{
			logError("Failed to find: {}", fileLocation.c_str());
			return Resource<Texture>::empty;
		}

		GLenum format = GL_RGB;
		if (bpp == 1)
			format = GL_RED;
		else if (bpp == 3)
			format = GL_RGB;
		else if (bpp == 4)
			format = GL_RGBA;


		Resource<Texture> texture = create2DTextureFromBuffer(width, height, format, format, GL_UNSIGNED_BYTE,
		{
			{ GL_TEXTURE_WRAP_S, GL_REPEAT},
			{ GL_TEXTURE_WRAP_T, GL_REPEAT},
			{ GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
			{ GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		}, data);

		auto& projectDir = Engine::get()->getProjectDirectory();
		stbi_write_png((projectDir + "/" + texture.getUID() + ".png").c_str(), width, height, bpp, data, width * bpp);
		Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(texture.getUID());

		
		//texture.get()->m_target = GL_TEXTURE_2D;
		//texture.get()->m_width = width;
		//texture.get()->m_height = height;
		//texture.get()->m_bitDepth = bpp;

		//// generate texture and bind it
		//glGenTextures(1, &texture.get()->m_id);
		//texture.get()->bind();

		//// sets the texture parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//// generate texture and mipmaps
		//glTexImage2D(GL_TEXTURE_2D, 0, format, texture.get()->m_width, texture.get()->m_height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		//// unbind texture and release the image.
		//texture.get()->unbind();
		stbi_image_free(data);

		return texture;
	});
}

Resource<Texture> Texture::create2DTextureFromBuffer(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, const void* data)
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->m_target = GL_TEXTURE_2D;
	texture.get()->m_width = width;
	texture.get()->m_height = height;

	// generate texture
	glGenTextures(1, &texture.get()->m_id);
	texture.get()->bind();

	for (auto& [paramKey, paramValue] : params)
	{
		glTexParameteri(GL_TEXTURE_2D, paramKey, paramValue);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

	

	texture.get()->unbind();

	return texture;
}

Resource<Texture> Texture::loadCubemapTexture(std::vector<std::string> faces)
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->m_target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &texture.get()->m_id);
	texture.get()->bind();

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



		//stbi_write_png(("Content/" + texture.getUID() + ".png").c_str(), width, height, nrChannels, data, texture.get()->m_width * texture.get()->m_bitDepth);
		//Engine::get()->getContext()->getProjectAssetRegistry()->addTexture(texture.getUID());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture.get()->unbind();

	return texture;
}

Resource<Texture> Texture::createCubemapTexture(int width, int height, int internalFormat, int format, int type)
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->m_target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &texture.get()->m_id);
	texture.get()->bind();

	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture.get()->unbind();

	return texture;
}

Resource<Texture> Texture::createCubemapTexture(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, bool createMipMaps)
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->m_target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &texture.get()->m_id);
	texture.get()->bind();

	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, nullptr);
	}
	for (auto& [paramKey, paramValue] : params)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, paramKey, paramValue);
	}

	if (createMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	texture.get()->unbind();

	return texture;
}

Resource<Texture> Texture::createDummyTexture()
{
	Resource<Texture> texture = Factory<Texture>::create();

	texture.get()->m_target = GL_TEXTURE_2D;
	texture.get()->m_width = 1;
	texture.get()->m_height = 1;

	// generate texture
	glGenTextures(1, &texture.get()->m_id);
	texture.get()->bind();

	unsigned char data[3] = { 255, 255, 255 }; // RGB values for white

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	texture.get()->unbind();

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
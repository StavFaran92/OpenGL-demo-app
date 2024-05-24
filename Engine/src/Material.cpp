#include "Material.h"

#include "Logger.h"
#include <GL\glew.h>

#include "Engine.h"
#include "Context.h"
#include "CommonTextures.h"

Material::Material()
{}

void Material::use(Shader& shader)
{
	setTexturesInShader(shader);
}

void Material::release()
{
	for (auto i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

Resource<Texture> Material::getTexture(Texture::Type textureType) const
{
	return m_textures.at(textureType);
}

bool Material::hasTexture(Texture::Type textureType) const
{
	auto iter = m_textures.find(textureType);
	return iter != m_textures.end() && iter->second.get();
}

void Material::setTextureInShader(Shader& shader, Texture::Type ttype, int slot)
{
	Resource<Texture> texture;
	if (hasTexture(ttype))
	{
		texture = getTexture(ttype);
	}
	else
	{
		texture = Engine::get()->getCommonTextures()->getTexture(CommonTextures::TextureType::WHITE_1X1);
	}

	// Activate texture unit i
	glActiveTexture(GL_TEXTURE0 + slot);

	// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
	glBindTexture(GL_TEXTURE_2D, texture.get()->getID());

	// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
	shader.setValue("material." + Texture::textureTypeToString(ttype), slot);
}

void Material::setTexturesInShader(Shader& shader)
{
	int index = 0;
	setTextureInShader(shader, Texture::Type::Albedo, index++);
	setTextureInShader(shader, Texture::Type::Normal, index++);
	setTextureInShader(shader, Texture::Type::Metallic, index++);
	setTextureInShader(shader, Texture::Type::Roughness, index++);
	setTextureInShader(shader, Texture::Type::AmbientOcclusion, index++);
}

void Material::setTexture(Texture::Type textureType, Resource<Texture> textureHandler)
{
	m_textures[textureType] = textureHandler;
}
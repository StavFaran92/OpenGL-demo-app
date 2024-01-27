#include "Material.h"

#include "Logger.h"
#include <GL\glew.h>

#include "Engine.h"
#include "Context.h"

Material::Material(float shine)
: m_shininess(shine)
{
	//auto texturediff = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png", Texture::Type::Diffuse);
	//if (!texturediff)
	//{
	//	logError("Failed to load resource");
	//	return;
	//}

	//setTexture(Texture::Type::Diffuse, std::shared_ptr<TextureHandler>(texturediff));

	//auto textureSpec = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png", Texture::Type::Specular);
	//if (!textureSpec)
	//{
	//	logError("Failed to load resource");
	//	return;
	//}

	//setTexture(Texture::Type::Specular, std::shared_ptr<TextureHandler>(textureSpec));
}

void Material::use(Shader& shader)
{
	SetTexturesInShader(shader);
}

void Material::release()
{
	for (auto i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

std::shared_ptr<TextureHandler> Material::getTexture(Texture::Type textureType) const
{
	return m_textures.at(textureType);
}

void Material::setTextureInShader(Shader& shader, Texture::Type ttype, int slot)
{
	auto texture = m_textures[ttype];
	if (!texture)
	{
		texture = Engine::get()->getContext()->getDummyTexture();
	}

	// Activate texture unit i
	glActiveTexture(GL_TEXTURE0 + slot);

	// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
	glBindTexture(GL_TEXTURE_2D, texture->getID());

	// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
	shader.setValue("material." + Texture::textureTypeToString(ttype), slot);
}

void Material::SetTexturesInShader(Shader& shader)
{
	int index = 0;
	setTextureInShader(shader, Texture::Type::Albedo, index++);
	setTextureInShader(shader, Texture::Type::Normal, index++);
	setTextureInShader(shader, Texture::Type::Metallic, index++);
	setTextureInShader(shader, Texture::Type::Roughness, index++);
	setTextureInShader(shader, Texture::Type::AmbientOcclusion, index++);
}

void Material::setReflection(bool enable)
{
	m_isReflective = enable;
}

void Material::setRefraction(bool enable)
{
	m_isRefractive = enable;
}

void Material::setTexture(Texture::Type textureType, std::shared_ptr<TextureHandler> textureHandler)
{
	m_textures[textureType] = textureHandler;
}

bool Material::isReflective() const
{
	return m_isReflective;
}
bool Material::isRefractive() const
{
	return m_isRefractive;
}
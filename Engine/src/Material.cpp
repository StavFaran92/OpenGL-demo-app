#include "Material.h"

#include "Logger.h"
#include <GL\glew.h>

#include "Engine.h"
#include "Context.h"

Material::Material(float shine)
: m_shininess(shine)
{
	auto texturediff = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png", Texture::Type::Diffuse);
	if (!texturediff)
	{
		logError("Failed to load resource");
		return;
	}

	setTexture(Texture::Type::Diffuse, std::shared_ptr<TextureHandler>(texturediff));

	auto textureSpec = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png", Texture::Type::Specular);
	if (!textureSpec)
	{
		logError("Failed to load resource");
		return;
	}

	setTexture(Texture::Type::Specular, std::shared_ptr<TextureHandler>(textureSpec));
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

void Material::SetTexturesInShader(Shader& shader)
{
	auto diffTexture = m_textures[Texture::Type::Diffuse];
	if (!diffTexture)
	{
		diffTexture = Engine::get()->getContext()->getDummyTexture();
	}

	// Activate texture unit i
	glActiveTexture(GL_TEXTURE0);

	// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
	glBindTexture(GL_TEXTURE_2D, diffTexture->getID());

	// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
	shader.setValue(("material." + Constants::g_textureDiffuse).c_str(), 0);

	auto specTexture = m_textures[Texture::Type::Specular];
	if (!specTexture)
	{
		specTexture = Engine::get()->getContext()->getDummyTexture();
	}

	// Activate texture unit i
	glActiveTexture(GL_TEXTURE0 + 1);

	// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
	glBindTexture(GL_TEXTURE_2D, specTexture->getID());

	// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
	shader.setValue(("material." + Constants::g_textureSpecular).c_str(), 1);
	

	auto normalTexture = m_textures[Texture::Type::Normal];
	if (!normalTexture)
	{
		normalTexture = Engine::get()->getContext()->getDummyTexture();
	}

	// Activate texture unit i
	glActiveTexture(GL_TEXTURE0 + 2);

	// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
	glBindTexture(GL_TEXTURE_2D, normalTexture->getID());

	// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
	shader.setValue(("material." + Constants::g_textureNormal).c_str(), 2);
	//shader.setValue("material.useNormal", true);
	

	// This causes skybox to not render
	// Iterate the mesh's textures
	//for (auto i = 0; i < textures.size(); i++)
	//{
	//	// get type count
	//	std::string count;
	//	auto type = textures[i]->getType();
	//	if (type == Texture::Type::Diffuse)
	//		count = std::to_string(diffuseNr++);
	//	else if (type == Texture::Type::Specular)
	//		count = std::to_string(specularNr++);

	//	// Get type as string
	//	auto typeStr = Texture::textureTypeToString(type);

	//	// Activate texture unit i
	//	glActiveTexture(GL_TEXTURE0 + i);

	//	// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
	//	glBindTexture(GL_TEXTURE_2D, textures[i]->getID());

	//	// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
	//	shader.setValue(("material." + typeStr + count).c_str(), i);
	//}
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
#include "DefaultMaterial.h"

#include "Logger.h"



DefaultMaterial::DefaultMaterial(float shine = 0) : Material(shine)
{
	//TODO refactor
//TODO optimize: can load textuer on startup and simply assign texture Ptr / ID
	auto texturediff = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	if (!texturediff)
	{
		logError("Failed to load resource");
		return;
	}
	texturediff->setType(Texture::Type::Diffuse);

	auto textureSpec = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	if (!textureSpec)
	{
		logError("Failed to load resource");
		return;
	}
	textureSpec->setType(Texture::Type::Specular);

	// Init the default material with default textures
	m_defaultTextureHandlers.push_back(std::shared_ptr<TextureHandler>(texturediff));
	m_defaultTextureHandlers.push_back(std::shared_ptr<TextureHandler>(textureSpec));
}

void DefaultMaterial::UseMaterial(Shader& shader)
{
	Material::UseMaterial(shader);

	shader.setValue("material.shininess", m_shininess);
}

DefaultMaterial::~DefaultMaterial()
{
}

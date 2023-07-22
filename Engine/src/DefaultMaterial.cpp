#include "DefaultMaterial.h"

#include "Logger.h"



DefaultMaterial::DefaultMaterial(float shine = 0) : Material(shine)
{
	//TODO refactor
//TODO optimize: can load textuer on startup and simply assign texture Ptr / ID
	auto texturediff = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png", Texture::Type::Diffuse);
	if (!texturediff)
	{
		logError("Failed to load resource");
		return;
	}

	auto textureSpec = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png", Texture::Type::Specular);
	if (!textureSpec)
	{
		logError("Failed to load resource");
		return;
	}

	// Init the default material with default textures
	m_defaultTextureHandlers.push_back(std::shared_ptr<TextureHandler>(texturediff));
	m_defaultTextureHandlers.push_back(std::shared_ptr<TextureHandler>(textureSpec));
}

void DefaultMaterial::use(Shader& shader)
{
	Material::use(shader);

	shader.setValue("material.shininess", m_shininess);
}

void DefaultMaterial::release()
{
	Material::release();
}

DefaultMaterial::~DefaultMaterial()
{
}

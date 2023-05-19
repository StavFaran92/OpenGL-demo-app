#include "Material.h"

#include "Logger.h"
#include <GL\glew.h>

Material::Material()
	:m_shininess(0)
{
}

Material::Material(float shine)
: m_shininess(shine)
{
}

void Material::UseMaterial(Shader& shader)
{
	if (!shader.IsMaterialsEnabled())
	{
		logError("Shader does not support material");
		return;
	}

	if (shader.IsTexturesEnabled())
	{
		SetTexturesInShader(shader);
	}
}

std::vector<const TextureHandler*> Material::getTextureHandlers() const
{
	std::vector<const TextureHandler*> textureHandlers;

	for (auto textureHandler : m_textureHandlers)
	{
		textureHandlers.emplace_back(textureHandler.get());
	}

	return textureHandlers;
}

void Material::addTextureHandler(TextureHandler* textureHandler)
{
	if (textureHandler == nullptr)
	{
		logError("Cannot add a null texture Handler to mesh.");
		return;
	}

	m_textureHandlers.push_back(std::shared_ptr<TextureHandler>(textureHandler));
}

void Material::addTextureHandlers(std::vector<TextureHandler*>& texturesHandlers)
{
	for (auto textureHandler : texturesHandlers)
	{
		addTextureHandler(textureHandler);
	}
}

Material::~Material()
{}

void Material::SetTexturesInShader(Shader & shader)
{
	// Initialized counters
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;

	// Iterate the mesh's textures
	for (auto i = 0; i < m_textureHandlers.size(); i++)
	{
		// get type count
		std::string count;
		auto type = m_textureHandlers[i]->getType();
		if (type == Texture::Type::Diffuse)
			count = std::to_string(diffuseNr++);
		else if (type == Texture::Type::Specular)
			count = std::to_string(specularNr++);

		// Get type as string
		auto typeStr = Texture::textureTypeToString(type);

		// Activate texture unit i
		glActiveTexture(GL_TEXTURE0 + i);

		// Binds iterated texture to target GL_TEXTURE_2D on texture unit i
		glBindTexture(GL_TEXTURE_2D, m_textureHandlers[i]->getID());

		// set sampler2D (e.g. material.diffuse3 to the currently active texture unit)
		shader.setInt(("material." + typeStr + count).c_str(), i);
	}
}

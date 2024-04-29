#pragma once

#include <memory>
#include <map>
#include "Core.h"

#include "TextureHandler.h"
#include "Shader.h"

class EngineAPI Material
{
public:
	Material();
	~Material() = default;

	virtual void use(Shader& shader);

	virtual void release();

	/**
	 * Gets the mesh's Textures.
	 *
	 * \param colors
	 */
	std::shared_ptr < TextureHandler >getTexture(Texture::Type textureType) const;
	bool hasTexture(Texture::Type textureType) const;

	void setTexture(Texture::Type textureType, std::shared_ptr<TextureHandler> textureHandler);

	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_specularIntensity);
	}

protected:
	void setTexturesInShader(Shader& shader);
	void setTextureInShader(Shader& shader, Texture::Type ttype, int slot);

protected:
	float m_specularIntensity = 0.f;
	float m_shininess = 32.f;

	std::map<Texture::Type, std::shared_ptr<TextureHandler>> m_textures;
};

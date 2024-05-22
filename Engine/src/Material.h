#pragma once

#include <memory>
#include <map>
#include "Core.h"

#include "Resource.h"
#include "Shader.h"
#include "Texture.h"

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
	Resource<Texture> getTexture(Texture::Type textureType) const;
	bool hasTexture(Texture::Type textureType) const;

	void setTexture(Texture::Type textureType, Resource<Texture> textureHandler);

	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_textures);
	}

protected:
	void setTexturesInShader(Shader& shader);
	void setTextureInShader(Shader& shader, Texture::Type ttype, int slot);

protected:
	std::map<Texture::Type, Resource<Texture>> m_textures;
};

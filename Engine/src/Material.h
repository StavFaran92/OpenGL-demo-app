#pragma once

#include <memory>
#include <map>
#include "Core.h"

#include "Resource.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureSampler.h"

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
	std::shared_ptr<TextureSampler> getSampler(Texture::TextureType textureType) const;
	void setSampler(Texture::TextureType textureType, std::shared_ptr<TextureSampler> sampler);

	bool hasTexture(Texture::TextureType textureType) const;

	void setTexture(Texture::TextureType textureType, Resource<Texture> textureHandler);

	std::vector<Resource<Texture>> getAllTextures() const;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_samplers);
	}

protected:
	void setTexturesInShader(Shader& shader);
	void setTextureInShader(Shader& shader, Texture::TextureType ttype, int slot);

protected:
	std::map<Texture::TextureType, std::shared_ptr<TextureSampler>> m_samplers;

	std::shared_ptr<TextureSampler> m_defaultSampler;
};

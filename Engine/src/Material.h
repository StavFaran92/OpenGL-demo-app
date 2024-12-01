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
	std::shared_ptr<TextureSampler> getSampler(Texture::Type textureType) const;
	void setSampler(Texture::Type textureType, std::shared_ptr<TextureSampler> sampler);

	bool hasTexture(Texture::Type textureType) const;

	void setTexture(Texture::Type textureType, Resource<Texture> textureHandler);

	std::vector<Resource<Texture>> getAllTextures() const;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_samplers);
	}

protected:
	void setTexturesInShader(Shader& shader);
	void setTextureInShader(Shader& shader, Texture::Type ttype, int slot);

protected:
	std::map<Texture::Type, std::shared_ptr<TextureSampler>> m_samplers;

	std::shared_ptr<TextureSampler> m_defaultSampler;
};

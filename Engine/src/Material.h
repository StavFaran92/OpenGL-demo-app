#pragma once

#include <memory>
#include <map>
#include "Core.h"

#include "TextureHandler.h"
#include "Shader.h"

class EngineAPI Material
{
public:
	Material(float shine = 32.f);

	virtual void use(Shader& shader);

	virtual void release();

	/**
	 * Gets the mesh's Textures.
	 *
	 * \param colors
	 */
	std::shared_ptr < TextureHandler >getTexture(Texture::Type textureType) const;

	void setReflection(bool enable);
	void setRefraction(bool enable);

	void setTexture(Texture::Type textureType, std::shared_ptr<TextureHandler> textureHandler);

	bool isReflective() const;
	bool isRefractive() const;

protected:
	void SetTexturesInShader(Shader& shader);
	void setTextureInShader(Shader& shader, Texture::Type ttype, int slot);

protected:
	float m_specularIntensity;
	float m_shininess;

	std::map<Texture::Type, std::shared_ptr<TextureHandler>> m_textures;

	bool m_isReflective = false;
	bool m_isRefractive = false;
};

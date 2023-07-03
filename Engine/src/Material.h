#pragma once

#include <memory>
#include "Core.h"

#include "TextureHandler.h"
#include "Shader.h"

class EngineAPI Material
{
public:
	Material(float shine = 0);

	virtual void use(Shader& shader);

	virtual void release();

	/**
	 * Add a texture to the mesh.
	 *
	 * \param texture	a texture raw pointer
	 */
	void addTextureHandler(TextureHandler* texture);

	/**
	 * Add multiple textures to the mesh.
	 *
	 * \param textures
	 */
	void addTextureHandlers(std::vector<TextureHandler*>& textures);

	/**
	 * Gets the mesh's Textures.
	 *
	 * \param colors
	 */
	std::vector<const TextureHandler*> getTextureHandlers() const;

	void setReflection(bool enable);
	void setRefraction(bool enable);

	bool isReflective() const;
	bool isRefractive() const;

protected:
	void SetTexturesInShader(Shader& shader);

protected:
	float m_specularIntensity;
	float m_shininess;

	/** Material texture Handlers */
	std::vector<std::shared_ptr<TextureHandler>>  m_defaultTextureHandlers;
	std::vector<std::shared_ptr<TextureHandler>>  m_textureHandlers;

	bool m_isReflective = false;
	bool m_isRefractive = false;
};

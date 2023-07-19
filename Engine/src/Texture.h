#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Core.h"
#include "ApplicationConstants.h"

class TextureHandler;

class EngineAPI Texture
{
public:
	enum class Type
	{
		Diffuse,
		Specular,

		None
	};

	/** Constructor */
	Texture();

	/** Copy constructor */
	Texture(const Texture& other);

	/**
	 * Create an empty texture.
	 * 
	 * \param width		The generated texture width 
	 * \param height	The generated texture height
	 * \return			A pointer to the generated texture
	 */
	static TextureHandler* createEmptyTexture(int width, int height);

	/**
	 * Create an empty texture.
	 *
	 * \param width		The generated texture width
	 * \param height	The generated texture height
	 * \return			A pointer to the generated texture
	 */
	static TextureHandler* createEmptyTexture(int width, int height, int internalFormat, int format, int type);

	/**
	 * Load a texture from a given file path.
	 * 
	 * \param fileLocation	The filepath location
	 * \param isFlipped		Should flip the texture vertically
	 * \return				A pointer to the loaded texture
	 */
	static TextureHandler* loadTextureFromFile(const std::string& fileLocation, Texture::Type type);

	/**
	 * Load a cubemap texture using the specified faces file locations.
	 * 
	 * \param faces		the faces file locations to load the cubemap using
	 * \return			A pointer to the loaded texture
	 */
	static TextureHandler* loadCubemapTexture(std::vector<std::string> faces);



	/**
	 * Converts a texture type to a string.
	 * 
	 * \param type	The texture type to convert
	 * \return		A string representation of the texture type.
	 */
	static std::string textureTypeToString(Type type);

	/** Binds the GPU to this texture */
	void bind() const;

	/** Unbinds this texture from the GPU */
	void unbind() const;

	void flip();
	bool isFlipped() const;
	unsigned int getID() const;

	inline void setSlot(int slot) { m_slot = slot; }

	/**  Destructor */
	~Texture();
private:
	void ClearTexture();

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot = 0;
	Type m_type;
	uint32_t m_target = 0;
	bool m_flipped = false;
};

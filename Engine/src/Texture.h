#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Core.h"


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

	/**
	 * Create an empty texture.
	 * 
	 * \param width		The generated texture width 
	 * \param height	The generated texture height
	 * \return		A pointer to the generated texture
	 */
	static std::shared_ptr<Texture> createEmptyTexture(int width, int height);

	/**
	 * Load a texture from a given file path.
	 * 
	 * \param fileLocation	The filepath location
	 * \param isFlipped		Should flip the texture vertically
	 * \return		A pointer to the loaded texture
	 */
	static std::shared_ptr<Texture> loadTextureFromFile(const std::string& fileLocation, bool isFlipped = false);
	static std::shared_ptr<Texture> loadCubemapTexture(std::vector<std::string> faces);

	/**
	 * Converts a texture type to a string.
	 * 
	 * \param type	The texture type to convert
	 * \return		A string representation of the texture type.
	 */
	static std::string textureTypeToString(Type type);

	/** Binds the GPU to this texture */
	void bind();

	/** Unbinds this texture from the GPU */
	void unbind();

	/**
	 * Sets the texture type.
	 * 
	 * \param type	The texture type to set
	 */
	void setType(Type type);

	/**
	 * Get the texture type.
	 * 
	 * \return Type of this texture
	 */
	Type getType() const;
	unsigned int getID() const;
	std::string getFilepath() const;
	uint32_t getTarget() const;

	/**  Destructor */
	~Texture();
private:
	void setTarget(uint32_t target);
	void ClearTexture();

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot;
	Type m_type;
	uint32_t m_target = 0;

	std::string m_fileLocation;
};

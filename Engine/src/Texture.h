#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Core.h"
#include "ApplicationConstants.h"
#include "Configurations.h"
#include "Resource.h"

class EngineAPI Texture
{
public:
	enum class Type
	{
		Diffuse,
		Specular,
		Normal,

		Albedo,
		Roughness,
		Metallic,
		AmbientOcclusion,

		None
	};

	struct TextureData
	{
		int width = 0;
		int height = 0;
		int bpp = 0;
		int internalFormat = 0;
		int format = 0;
		int target = 0;
		int type = 0;
		std::map<int, int> params;
		bool genMipMap = false;
		void* data = nullptr;
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
	static Resource<Texture> createEmptyTexture(int width, int height);

	/**
	 * Create an empty texture.
	 *
	 * \param width		The generated texture width
	 * \param height	The generated texture height
	 * \return			A pointer to the generated texture
	 */
	static Resource<Texture> createEmptyTexture(int width, int height, int internalFormat, int format, int type);

	/**
	 * Load a texture from a given file path.
	 * 
	 * \param fileLocation	The filepath location
	 * \param isFlipped		Should flip the texture vertically
	 * \return				A pointer to the loaded texture
	 */
	static Resource<Texture> create2DTextureFromFile(const std::string& fileLocation, bool flip = FLIP_TEXTURE);

	/**
	 * Create a texture using predefined data
	 *
	 * \return				A pointer to the loaded texture
	 */
	static Resource<Texture> create2DTextureFromBuffer(const TextureData& textureData);

	static Resource<Texture> create2DTextureFromBuffer(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, void* data);

	static Resource<Texture> createDummyTexture(unsigned char color[3]);

	//static Texture::TextureData extractTextureDataFromFile(const std::string& fileLocation);

	void build(const TextureData& textureData);

	int getWidth() const;
	int getHeight() const;
	int getBitDepth() const;

	void setData(int xoffset, int yoffset, int width, int height, const void* data);

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
	friend class Cubemap;

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot = 0;
	Type m_type;
	uint32_t m_target = 0;
	bool m_flipped = false;
};

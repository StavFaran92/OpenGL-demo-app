#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Core.h"
#include "ApplicationConstants.h"
#include "Configurations.h"
#include "Resource.h"

class EngineAPI Cubemap
{
public:
	struct CubemapData
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

		void* data[6]{nullptr};
	};

	/** Constructor */
	Cubemap();

	/** Copy constructor */
	Cubemap(const Cubemap& other);

	/**
	 * Load a cubemap texture using the specified faces file locations.
	 * 
	 * \param faces		the faces file locations to load the cubemap using
	 * \return			A pointer to the loaded texture
	 */
	static Resource<Cubemap> createCubemapFromFile(const std::vector<std::string>& fileLocations);
	static Resource<Cubemap> createCubemapFromFile(const std::string& fileLocation);
	static Resource<Cubemap> createCubemapFromBuffer(const CubemapData& cubemapData);

	static Resource<Cubemap> createEmptyCubemap(int width, int height, int internalFormat, int format, int type);

	static Resource<Cubemap> createEmptyCubemap(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, bool createMipMaps = false);

	static Cubemap::CubemapData Cubemap::extractCubemapDataFromMultipleFiles(const std::vector<std::string>& files);

	static Resource<Texture> build(const CubemapData& textureData);

	/** Binds the GPU to this texture */
	void bind() const;

	/** Unbinds this texture from the GPU */
	void unbind() const;

	void flip();
	bool isFlipped() const;
	unsigned int getID() const;

	inline void setSlot(int slot) { m_slot = slot; }

	/**  Destructor */
	~Cubemap();
private:
	void clear();

private:
	uint32_t m_id;
	int m_width, m_height, m_bitDepth;
	int m_slot = 0;
	uint32_t m_target = 0;
	bool m_flipped = false;
};

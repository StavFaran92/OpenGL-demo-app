#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Core.h"
#include "ApplicationConstants.h"
#include "Configurations.h"
#include "Resource.h"
#include "Texture.h"

class EngineAPI Cubemap
{
public:
	//struct CubemapData
	//{
	//	int width = 0;
	//	int height = 0;
	//	int bpp = 0;
	//	int internalFormat = 0;
	//	int format = 0;
	//	int target = 0;
	//	int type = 0;
	//	std::map<int, int> params;
	//	bool genMipMap = false;

	//	void* data[6]{nullptr};
	//};

	static Resource<Texture> createCubemapFromCubemapFiles(const std::vector<std::string>& fileLocations);
	static Resource<Texture> createCubemapFromEquirectangularFile(const std::string& fileLocation);

	static Resource<Texture> createCubemapFromBuffer(const Texture::TextureData& cubemapData);

	static Resource<Texture> createEmptyCubemap(int width, int height, int internalFormat, int format, int type);
	static Resource<Texture> createEmptyCubemap(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, bool createMipMaps = false);

	static Texture::TextureData extractCubemapDataFromEquirectangularFile(const std::string& fileLocation);
	static Texture::TextureData extractCubemapDataFromCubemapFiles(const std::vector<std::string>& files);
	static void saveEquirectangularMap(Resource<Texture> equirectangularMap);

	static Resource<Texture> build(const Texture::TextureData& textureData);
};

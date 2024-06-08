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

	static Resource<Texture> createCubemapFromCubemapFiles(const std::vector<std::string>& fileLocations);
	static Resource<Texture> createCubemapFromEquirectangularFile(const std::string& fileLocation);

	static Resource<Texture> createCubemapFromBuffer(const CubemapData& cubemapData);

	static Resource<Texture> createEmptyCubemap(int width, int height, int internalFormat, int format, int type);
	static Resource<Texture> createEmptyCubemap(int width, int height, int internalFormat, int format, int type, std::map<int, int> params, bool createMipMaps = false);

	static Cubemap::CubemapData extractCubemapDataFromEquirectangularFile(const std::string& fileLocation);
	static Cubemap::CubemapData extractCubemapDataFromCubemapFiles(const std::vector<std::string>& files);

	static Resource<Texture> build(const CubemapData& textureData);
};

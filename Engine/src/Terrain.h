#pragma once

#include <string>

#include "Core.h"

class EngineAPI Terrain
{
public:
	static Terrain generateTerrain(int width, int height, int scale, const std::string& heightMapFilepath);


};
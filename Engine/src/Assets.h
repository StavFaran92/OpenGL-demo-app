#pragma once

#include "Engine.h"
#include "Resource.h"
#include "Texture.h"

class Assets
{
public:
	Assets();

	Resource<Texture> importTexture(const std::string& path);
};
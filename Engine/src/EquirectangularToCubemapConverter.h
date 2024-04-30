#pragma once

class Scene;

#include "Resource.h"

class EquirectangularToCubemapConverter
{
public:
	static Resource<Texture> convert(Resource<Texture> equirectangularTexture, Scene* scene);
};
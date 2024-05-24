#pragma once

class Scene;

#include "Resource.h"

class EquirectangularToCubemapConverter
{
public:
	static Resource<Texture> fromEquirectangularToCubemap(Resource<Texture> equirectangularTexture, Scene* scene);
	static Resource<Texture> fromCubemapToEquirectangular(Resource<Texture> cubemapTexture, Scene* scene);
};
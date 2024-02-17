#pragma once

class Scene;
class TextureHandler;

class EquirectangularToCubemapConverter
{
public:
	static TextureHandler* convert(TextureHandler* equirectangularTexture, Scene* scene);
};
#pragma once

class Scene;
class TextureHandler;

class EquirectangulatToCubemapConverter
{
public:
	static void convert(TextureHandler* equirectangularTexture, Scene* scene);
};
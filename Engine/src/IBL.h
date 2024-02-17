#pragma once

class Scene;
class TextureHandler;

class IBL
{
public:
	static TextureHandler* generateIrradianceMap(TextureHandler* cubemap, Scene* scene);
};

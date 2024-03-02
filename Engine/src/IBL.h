#pragma once

class Scene;
class TextureHandler;

class IBL
{
public:
	static TextureHandler* generateIrradianceMap(TextureHandler* cubemap, Scene* scene);
	static TextureHandler* generatePrefilterEnvMap(TextureHandler* cubemap, Scene* scene);
	static TextureHandler* generateBRDFIntegrationLUT(Scene* scene);
};

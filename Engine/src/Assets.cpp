#include "Assets.h"

Assets::Assets()
{
	Engine::get()->registerSubSystem<Assets>(this);
}

Resource<Texture> Assets::importTexture(const std::string& path)
{
	return Resource<Texture>();
}

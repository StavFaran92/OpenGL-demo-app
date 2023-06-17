#pragma once

#include "Quad.h"

class Entity;
class Scene;

class ScreenQuad
{
public:
	static std::shared_ptr<Entity> GenerateScreenQuad(Scene* scene);
};


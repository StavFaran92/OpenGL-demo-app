#pragma once

#include <memory>

#include "Core.h"

class Entity;
class Scene;

class EngineAPI ShapeFactory
{
public:
	static Entity createBox(Scene* scene);
	static Entity createPlane(Scene* scene);
	static Entity createSphere(Scene* scene, float radius, int sectors, int stacks);

private:
	static Entity createEntity(Scene* scene);
};
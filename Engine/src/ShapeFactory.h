#pragma once

#include <memory>

#include "Core.h"

class Entity;
class Scene;

class EngineAPI ShapeFactory
{
public:
	static std::shared_ptr<Entity> createBox(Scene* scene);
	static std::shared_ptr<Entity> createQuad(Scene* scene);
	static std::shared_ptr<Entity> createSphere(Scene* scene, float radius, int sectors, int stacks);

private:
	static std::shared_ptr<Entity> createEntity(Scene* scene);
};
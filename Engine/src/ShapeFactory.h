#pragma once

#include <memory>
#include <string>

#include "Core.h"

class Entity;
class SGE_Regsitry;

class EngineAPI ShapeFactory
{
public:
	static Entity createBox(SGE_Regsitry* registry);
	static void createBox(Entity& e, SGE_Regsitry* registry);
	static Entity createQuad(SGE_Regsitry* registry);
	static Entity createSphere(SGE_Regsitry* registry);

private:
	static Entity createEntity(SGE_Regsitry* registry, const std::string& name);
};
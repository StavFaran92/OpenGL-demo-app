#pragma once
#include "Core.h"

#include "Mesh.h"

class EngineAPI Sphere
{
public:
	static Mesh* createMesh(float radius, int sectors, int stacks);
};

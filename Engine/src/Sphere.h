#pragma once
#include "Core.h"

#include "Model.h"

class EngineAPI Sphere : public Model
{
public:
	static Sphere* generateSphere(float radius, int sectors, int stacks);
private:
	static std::shared_ptr<Mesh> generateMesh(float radius, int sectors, int stacks);
};

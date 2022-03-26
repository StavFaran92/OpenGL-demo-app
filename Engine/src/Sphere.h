#pragma once

#include "Model.h"

class Sphere : public Model
{
public:
	static Mesh* generateSphere(float radius, int sectors, int stacks);
};

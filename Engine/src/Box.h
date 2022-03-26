#pragma once

#include "Model.h"

class Box : public Model
{
public:
	static Box* generateBox();
	static Mesh* generateMesh();
private:
};


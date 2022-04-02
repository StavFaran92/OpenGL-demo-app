#pragma once
#include "Core.h"
#include "Model.h"

class EngineAPI Quad : public Model
{
	friend class Model;
public:
	static Quad* generateQuad();
	static Mesh* generateMesh();

	static ModelBuilder builder();
};


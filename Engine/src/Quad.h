#pragma once

#include "Model.h"

class Quad : public Model
{
public:
	static Quad* generateQuad();
private:
	static std::shared_ptr<Mesh> generateMesh();
};


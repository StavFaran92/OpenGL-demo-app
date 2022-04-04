#pragma once

#include "Core.h"
#include "Model.h"

class EngineAPI Box : public Model
{
public:
	static Box* generateBox();
	static Mesh* generateMesh();

	MeshBuilder* createMeshBuilder() override;
private:
};


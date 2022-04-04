#pragma once
#include "Core.h"
#include "Model.h"

class MeshBuilder;

class EngineAPI Quad : public Model
{
public:
	MeshBuilder* createMeshBuilder() override;
};


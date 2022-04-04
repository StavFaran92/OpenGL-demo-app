#pragma once

#include "Core.h"
#include "Model.h"

class EngineAPI Box : public Model
{
public:
	MeshBuilder* createMeshBuilder() override;
};


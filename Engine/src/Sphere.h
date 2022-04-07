#pragma once
#include "Core.h"

#include "Model.h"

class EngineAPI Sphere : public Model
{
public:
	Sphere(float radius, int sectors, int stacks)
		: m_radius(radius), m_sectors(sectors), m_stacks(stacks) {};

	MeshBuilder* createMeshBuilder() override;
private:
	const int m_radius = 0;
	const int m_sectors = 0;
	const int m_stacks = 0;
};

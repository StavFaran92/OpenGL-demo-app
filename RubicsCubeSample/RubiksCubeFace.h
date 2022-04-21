#pragma once

#include "sge.h"

#include "RubiksCube.h"
#include "RubiksCubeConfigurations.h"

#include <vector>
#include <memory>

class RubiksCubeEnt;

using namespace rubiksCube;

/**
 * This class represesnts a rubiks cube face,
 * a face is contained of inner cubes and the sorrounding bounds: Top, Bottom, Left and Right.
 */
class RubiksCubeFace : public Object3D
{
public:
	RubiksCubeFace(Axis axis, int index, int size)
		: m_axis(axis), m_index(index), m_size(size)
	{
		m_cubes = std::vector<RubiksCubeEnt*>(size*size, 0);
	}

	void rotateCubes();
	static void cyclic_roll(RubiksCubeEnt* a, RubiksCubeEnt* b, RubiksCubeEnt* c, RubiksCubeEnt* d);

	void addCube(int x, int y, RubiksCubeEnt* cube);
	void removeCube(int x, int y);
	std::vector<RubiksCubeEnt*> getCubes();

private:
	std::vector<RubiksCubeEnt*> m_cubes;

	const Axis m_axis;
	const int m_index = 0;
	const int m_size = 0;
};
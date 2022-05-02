#pragma once

#include "sge.h"

#include "RubiksCubeConfigurations.h"

using namespace rubiksCube;

class RubiksCubeFace;
class RubiksCubeEnt;
class RubiksCubeController;

class RubiksCube
{
public:
	RubiksCube();
	void init(size_t size = 3);

	void rotateFace(Axis axis, int index, Shift shift);

	//Debug purposes
	std::vector<std::shared_ptr<RubiksCubeEnt>> getCubes()
	{
		return m_cubes;
	}

	void print(const std::string& padding = "") const;

	void onEntityPicked(RubiksCubeEnt* ent);

	void setController(RubiksCubeController* controller);
private:
	static std::shared_ptr<RubiksCubeEnt> createRubiksCubeBox(RubiksCube* rubiksCube);
	RubiksCubeEnt* getCube(int i, int j, int k) const;

private:
	std::vector<std::shared_ptr<RubiksCubeEnt>> m_cubes;
	std::map<std::pair<Axis, int>, std::shared_ptr<RubiksCubeFace>> m_faces;
	size_t m_size = 0;

	bool m_isRotating = false;

	RubiksCubeController* m_controller = nullptr;
};



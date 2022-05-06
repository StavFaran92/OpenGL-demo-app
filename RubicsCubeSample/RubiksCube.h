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
	~RubiksCube();

	void init(size_t size = 3);

	void rotateFace(Axis axis, int index, Shift shift);

	//Debug purposes
	std::vector<ObjectHandler<RubiksCubeEnt>> getCubes()
	{
		return m_cubes;
	}

	void print(const std::string& padding = "") const;

	void onEntityPicked(RubiksCubeEnt* ent);

	void setController(RubiksCubeController* controller);

	void shuffle();

	size_t getSize() const;
private:
	static ObjectHandler<RubiksCubeEnt> createRubiksCubeBox(RubiksCube* rubiksCube);
	RubiksCubeEnt* getCube(int i, int j, int k) const;
	void rotateFaceFast(ObjectHandler<RubiksCubeFace> face, Axis axis, Shift shift);
private:
	std::vector<ObjectHandler<RubiksCubeEnt>> m_cubes;
	std::map<std::pair<Axis, int>, ObjectHandler<RubiksCubeFace>> m_faces;
	size_t m_size = 0;

	bool m_isRotating = false;

	RubiksCubeController* m_controller = nullptr;
};



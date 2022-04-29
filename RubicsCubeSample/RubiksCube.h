#pragma once

#include "sge.h"

#include "RubiksCubeConfigurations.h"

using namespace rubiksCube;

class RubiksCubeFace;
class RubiksCubeEnt;

class RubiksCube : public Model
{
public:
	RubiksCube();
	void init(size_t size = 3);

	void update(float deltaTime) override;

	void rotateFace(Axis axis, int index, Shift shift);

	//Debug purposes
	std::vector<RubiksCubeEnt*> getCubes()
	{
		return m_cubes;
	}

	void print(const std::string& padding = "") const;

	
protected:
	void draw(IRenderer& renderer, Shader* shader = nullptr) override;
private:
	static RubiksCubeEnt* createRubiksCubeBox();
	RubiksCubeEnt* getCube(int i, int j, int k) const;

private:
	std::vector<RubiksCubeEnt*> m_cubes;
	std::map<std::pair<Axis, int>, RubiksCubeFace*> m_faces;
	size_t m_size = 0;

	std::vector<std::function<bool(float)>> m_asyncCallbacks;

	bool m_isRotating = false;
};



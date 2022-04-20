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

	//void rotateFront(Shift dir);
	//void rotateRight(Shift dir);
	//void rotateTop(Shift dir);
	//void rotateBack(Shift dir);
	//void rotateLeft(Shift dir);
	//void rotateBottom(Shift dir);

	//Testing purposes
	std::vector<RubiksCubeEnt*> getCubes()
	{
		return m_cubes;
	}

	
protected:
	void draw(IRenderer& renderer, Shader* shader = nullptr) override;
private:
	static RubiksCubeEnt* createRubiksCubeBox();
	RubiksCubeEnt* getCube(int i, int j, int k) const;

private:
	//RubiksCubeFace* m_faceFront  = nullptr;
	//RubiksCubeFace* m_faceRight  = nullptr;
	//RubiksCubeFace* m_faceTop    = nullptr;
	//RubiksCubeFace* m_faceBack   = nullptr;
	//RubiksCubeFace* m_faceLeft   = nullptr;
	//RubiksCubeFace* m_faceBottom = nullptr;

	std::vector<RubiksCubeEnt*> m_cubes;
	std::map<std::pair<Axis, int>, RubiksCubeFace*> m_faces;
	size_t m_size = 0;

	std::vector<std::function<bool(float)>> m_asyncCallbacks;
};


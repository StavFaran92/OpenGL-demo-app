#pragma once

class RubiksCube;
class RubiksCubeFace;
class RubiksCubeEnt;

class RubiksCubeController
{
public:
	void init(RubiksCube* rubiksCube);
	void setFace(RubiksCubeFace* face);
	void setPickedEnt(RubiksCubeEnt* ent);
private:
	RubiksCube* m_rubiksCube = nullptr;
	RubiksCubeFace* m_face = nullptr;
	RubiksCubeEnt* m_pickedEnt = nullptr;
};


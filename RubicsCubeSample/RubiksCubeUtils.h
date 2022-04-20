#pragma once

class RubiksCubeEnt;

class RubiksCubeUtils
{
public:
	static void rotateCubesInFace(RubiksCubeEnt** cubes, size_t size);
	static void cyclic_roll(RubiksCubeEnt* a, RubiksCubeEnt* b, RubiksCubeEnt* c, RubiksCubeEnt* d);



};
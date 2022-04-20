#include "RubiksCubeUtils.h"

#include <vector>
#include "RubiksCubeEnt.h"

//void RubiksCubeUtils::cyclic_roll(RubiksCubeEnt* a, RubiksCubeEnt* b, RubiksCubeEnt* c, RubiksCubeEnt* d)
//{
//	int temp = a;
//	a = b;
//	b = c;
//	c = d;
//	d = temp;
//}
//
//void RubiksCubeUtils::rotateCubesInFace(RubiksCubeEnt** cubes, size_t size)
//{
//	for (int i = 0; i < size / 2; i++)
//		for (int j = 0; j < (size + 1) / 2; j++)
//			cyclic_roll(cubes[i * size + j], cubes[(size - 1 - j) * size + i], cubes[(size - 1 - i) * size + size - 1 - j], cubes[j * size + size - 1 - i]);
//}

#pragma once

#include "sge.h"

#include "RubiksCubeConfigurations.h"

class RubiksCubeFace;

using namespace rubiksCube;

class RubiksCubeEnt : public Box
{
public:
	struct FaceData
	{
		RubiksCubeFace* face;
		int x;
		int y;
	};
	static void swapFaces(RubiksCubeEnt* a, RubiksCubeEnt* b);

	void setFaceData(Axis axis, RubiksCubeFace* face, int x, int y);
	FaceData getFaceData(Axis axis);
	void removeFace(Axis axis);

private:
	std::map<Axis, FaceData> m_faces;
};

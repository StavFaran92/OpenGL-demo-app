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
	void setRCID(int rcid);
	int getRCID();

	void print(const std::string& padding = "") const;

private:
	std::map<Axis, FaceData> m_faces;
	int m_RCID;
};

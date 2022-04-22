#include "RubiksCubeEnt.h"

#include "RubiksCubeFace.h"

void RubiksCubeEnt::swapFaces(RubiksCubeEnt* a, RubiksCubeEnt* b)
{
	// Swap faces For X Y and Z
	{
		auto faceData_a = a->getFaceData(Axis::X);
		auto faceData_b = b->getFaceData(Axis::X);

		faceData_a.face->removeCube(faceData_a.x, faceData_a.y);
		faceData_b.face->removeCube(faceData_b.x, faceData_b.y);

		faceData_a.face->addCube(faceData_a.x, faceData_a.y, b);
		faceData_b.face->addCube(faceData_b.x, faceData_b.y, a);
	}

	{
		auto faceData_a = a->getFaceData(Axis::Y);
		auto faceData_b = b->getFaceData(Axis::Y);

		faceData_a.face->removeCube(faceData_a.x, faceData_a.y);
		faceData_b.face->removeCube(faceData_b.x, faceData_b.y);

		faceData_a.face->addCube(faceData_a.x, faceData_a.y, b);
		faceData_b.face->addCube(faceData_b.x, faceData_b.y, a);

	}

	{
		auto faceData_a = a->getFaceData(Axis::Z);
		auto faceData_b = b->getFaceData(Axis::Z);

		faceData_a.face->removeCube(faceData_a.x, faceData_a.y);
		faceData_b.face->removeCube(faceData_b.x, faceData_b.y);

		faceData_a.face->addCube(faceData_a.x, faceData_a.y, b);
		faceData_b.face->addCube(faceData_b.x, faceData_b.y, a);
	}
}

void RubiksCubeEnt::setFaceData(Axis axis, RubiksCubeFace* face, int x, int y)
{
	m_faces[axis] = RubiksCubeEnt::FaceData{ face, x, y };

}

RubiksCubeEnt::FaceData RubiksCubeEnt::getFaceData(Axis axis)
{
	return m_faces[axis];
}

void RubiksCubeEnt::removeFace(Axis axis)
{
	m_faces[axis].face = nullptr;
	m_faces[axis].x = 0;
	m_faces[axis].y = 0;
}

void RubiksCubeEnt::print(const std::string& padding) const
{
	std::cout << padding << "Ent: " <<  this << std::endl;
}

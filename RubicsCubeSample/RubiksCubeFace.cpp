#include "RubiksCubeFace.h"

#include "RubiksCubeEnt.h"

void RubiksCubeFace::addCube(int x, int y, RubiksCubeEnt* cube)
{
	m_cubes[x * m_size + y] = cube;

	cube->setFaceData(m_axis, this, x, y);

	addChildren(cube);
}

void RubiksCubeFace::removeCube(int x, int y)
{
	auto cube = m_cubes[x * m_size + y];

	cube->removeFace(m_axis);

	removeChildren(cube);

	m_cubes[x * m_size + y] = nullptr;
}

std::vector<RubiksCubeEnt*> RubiksCubeFace::getCubes()
{
	return m_cubes;
}

void RubiksCubeFace::cyclic_roll(RubiksCubeEnt* a, RubiksCubeEnt* b, RubiksCubeEnt* c, RubiksCubeEnt* d)
{
	auto temp_faceDataX = a->getFaceData(Axis::X);
	auto temp_faceDataY = a->getFaceData(Axis::Y);
	auto temp_faceDataZ = a->getFaceData(Axis::Z);

	RubiksCubeEnt::swapFaces(a, b);
	RubiksCubeEnt::swapFaces(b, c);
	RubiksCubeEnt::swapFaces(c, d);

	d->setFaceData(Axis::X, temp_faceDataX.face, temp_faceDataX.x, temp_faceDataX.y);
	d->setFaceData(Axis::Y, temp_faceDataY.face, temp_faceDataY.x, temp_faceDataY.y);
	d->setFaceData(Axis::Z, temp_faceDataZ.face, temp_faceDataZ.x, temp_faceDataZ.y);
}

void RubiksCubeFace::rotateCubes(Shift shift)
{
	for (int i = 0; i < m_size / 2; i++)
		for (int j = 0; j < (m_size + 1) / 2; j++)
		{
			if(shift == Shift::CCW)
				cyclic_roll(m_cubes[i * m_size + j], m_cubes[(m_size - 1 - j) * m_size + i], m_cubes[(m_size - 1 - i) * m_size + m_size - 1 - j], m_cubes[j * m_size + m_size - 1 - i]);
			else
				cyclic_roll(m_cubes[j * m_size + m_size - 1 - i], m_cubes[(m_size - 1 - i) * m_size + m_size - 1 - j], m_cubes[(m_size - 1 - j) * m_size + i], m_cubes[i * m_size + j]);
		}
}

void RubiksCubeFace::print(const std::string& padding) const
{
	std::cout << padding << "Face: " << this << std::endl;

	for (auto cube : m_cubes)
	{
		cube->print(padding + g_padding);
	}
}
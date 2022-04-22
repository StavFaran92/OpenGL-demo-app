#include "RubiksCube.h"
#include "RubiksCubeFace.h"
#include "RubiksCubeEnt.h"

using namespace rubiksCube;

float colors[108] = {
	//Red
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	//Green
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,

	//Blue
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,

	//Yellow
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,

	//Purple
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,

	//White
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
};

void RubiksCube::print(const std::string& padding) const
{
	std::cout << padding << "Cube: " << this << std::endl;

	std::cout << "Cube entitites" << std::endl;
	for (int i = 0; i < m_cubes.size(); i++)
	{
		std::cout << "[" << std::to_string(i) << "]";
		m_cubes[i]->print();
		std::cout << std::endl;
	}

	std::cout << "Faces X" << std::endl;
	for (int i = 0; i < m_size; i++)
	{
		auto face = m_faces.at({ Axis::X, i });
		
		face->print(padding + g_padding);
	}

	std::cout << "Faces Y" << std::endl;
	for (int i = 0; i < m_size; i++)
	{
		auto face = m_faces.at({ Axis::Y, i });

		face->print(padding + g_padding);
	}

	std::cout << "Faces Z" << std::endl;
	for (int i = 0; i < m_size; i++)
	{
		auto face = m_faces.at({ Axis::Z, i });

		face->print(padding + g_padding);
	}
}

void RubiksCube::draw(IRenderer& renderer, Shader* shader)
{
	Model::draw(renderer, shader);
}

void RubiksCube::update(float deltaTime)
{
	Model::update(deltaTime);
}


void RubiksCube::rotateFace(Axis axis, int index, Shift shift)
{
	if (m_isRotating)
	{
		return;
	}

	m_isRotating = true;

	if (index < 0 || index > m_size - 1)
	{
		std::cout << "index out of bounds specified: " << index << std::endl;
		return;
	}

	glm::vec3 dir(1);
	switch (axis)
	{
	case Axis::X:
		dir = glm::vec3(1, 0, 0);
		break;
	case Axis::Y:
		dir = glm::vec3(0, 1, 0);
		break;
	case Axis::Z:
		dir = glm::vec3(0, 0, 1);
		break;
	}

	int angle = 0;
	dir = (shift == Shift::CW) ? dir : -dir;

	auto face = m_faces.at({ axis, index });

	//Rotate geometric representation of cubes
	Engine::get()->getContext()->getActiveScene()->addCoroutine([&, face, angle, dir, shift, axis](float deltaTime) mutable
	{
		face->rotate(dir, 1);

		angle += 1;

		if (angle >= 90)
		{
			// Hack to solve The Y axis reverse rotation
			if (axis == Axis::Y)
			{
				shift = (shift == Shift::CW) ? Shift::CCW : Shift::CW;
			}

			// Rotate cubes physically (i.e. replace the cube' attached face according to rotation)
			face->rotateCubes(shift);

			m_isRotating = false;

			return true;
		}

		return false;
	});



}

RubiksCubeEnt* RubiksCube::createRubiksCubeBox()
{
	TextureHandler* textureHandlerDiffse = Texture::loadTextureFromFile(g_rubiksCubeTexture);
	textureHandlerDiffse->setType(Texture::Type::Diffuse);

	TextureHandler* textureHandlerSpecular = Texture::loadTextureFromFile(g_rubiksCubeTexture);
	textureHandlerSpecular->setType(Texture::Type::Specular);

	return dynamic_cast<RubiksCubeEnt*>(ModelBuilder::builder<RubiksCubeEnt>()
	.getMeshBuilder()
	.setColors(colors, 36)
	.addTextureHandler(textureHandlerDiffse)
	.addTextureHandler(textureHandlerSpecular)
	.getModelBuilder()
	.build());
}

RubiksCubeEnt* RubiksCube::getCube(int x, int y, int z) const
{
	return m_cubes[x * m_size * m_size + y * m_size + z];
}

RubiksCube::RubiksCube()
{
}

void RubiksCube::init(size_t size)
{
	if (size < 2)
		size = 2;

	m_size = size;

	// generate cubes
	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			for (int k = 0; k < m_size; k++)
			{
				auto cube = createRubiksCubeBox();
				cube->setRCID(i * size * size + j * size + k);
				cube->translate(i, j, k);
				m_cubes.push_back(cube);
			}
		}
	}

	// Create X aligned axis faces
	for (int i = 0; i < m_size; i++)
	{
		auto face = new RubiksCubeFace(Axis::X, i, m_size);
		m_faces[{ Axis::X , i}] = face;
		face->translate(i, (float)(m_size-1) / 2, (float)(m_size - 1) / 2);
		for (int j = 0; j < m_size; j++)
		{
			for (int k = 0; k < m_size; k++)
			{
				face->addCube(j, k, getCube(i, j, k));
			}
		}
	}

	// Create Y aligned axis faces
	for (int i = 0; i < m_size; i++)
	{
		auto face = new RubiksCubeFace(Axis::Y, i, m_size);
		m_faces[{ Axis::Y , i }] = face;
		face->translate((float)(m_size - 1) / 2, i , (float)(m_size - 1) / 2);
		for (int j = 0; j < m_size; j++)
		{
			for (int k = 0; k < m_size; k++)
			{
				face->addCube(j, k, getCube(j, i, k));
			}
		}
	}

	// Create Z aligned axis faces
	for (int i = 0; i < m_size; i++)
	{
		auto face = new RubiksCubeFace(Axis::Z, i, m_size);
		m_faces[{ Axis::Z, i }] = face;
		face->translate((float)(m_size - 1) / 2, (float)(m_size - 1) / 2, i);
		for (int j = 0; j < m_size; j++)
		{
			for (int k = 0; k < m_size; k++)
			{
				face->addCube(j, k, getCube(j, k, i));
			}
		}
	}
}
#include "RubiksCube.h"

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

void RubiksCube::draw(IRenderer& renderer, Shader* shader)
{
	Model::draw(renderer, shader);
}

Model* RubiksCube::createRubiksCubeBox()
{
	auto texture1 = Texture::loadTextureFromFile("Resources\\Textures\\plane.png");
	texture1->setType(Texture::Type::Diffuse);

	auto texture2 = Texture::loadTextureFromFile("Resources\\Textures\\plane.png");
	texture2->setType(Texture::Type::Specular);

	return ModelBuilder::builder<Box>()
	.getMeshBuilder()
	.setColors(colors, 36)
	.addTexture(texture1)
	.addTexture(texture2)
	.getModelBuilder()
	.build();
}

Model* RubiksCube::getCube(int x, int y, int z) const
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
				cube->translate(i, j, k);
				m_cubes.push_back(cube);
			}
		}
	}

	// Front
	m_faceFront = new Object3D();
	m_faceFront->translate(1, 1, 0);

	m_faceFront->addChildren(getCube(0,0,0));
	m_faceFront->addChildren(getCube(1,0,0));
	m_faceFront->addChildren(getCube(2,0,0));

	m_faceFront->addChildren(getCube(0,1,0));
	m_faceFront->addChildren(getCube(1,1,0));
	m_faceFront->addChildren(getCube(2,1,0));

	m_faceFront->addChildren(getCube(0,2,0));
	m_faceFront->addChildren(getCube(1,2,0));
	m_faceFront->addChildren(getCube(2,2,0));

	// Right
	m_faceRight = new Object3D();
	m_faceRight->translate(2, 1, 1);

	m_faceRight->addChildren(getCube(2, 0, 0));
	m_faceRight->addChildren(getCube(2, 0, 1));
	m_faceRight->addChildren(getCube(2, 0, 2));

	m_faceRight->addChildren(getCube(2, 1, 0));
	m_faceRight->addChildren(getCube(2, 1, 1));
	m_faceRight->addChildren(getCube(2, 1, 2));

	m_faceRight->addChildren(getCube(2, 2, 0));
	m_faceRight->addChildren(getCube(2, 2, 1));
	m_faceRight->addChildren(getCube(2, 2, 2));

	// Top
	m_faceTop = new Object3D();
	m_faceTop->translate(1, 2, 1);

	m_faceTop->addChildren(getCube(0, 2, 0));
	m_faceTop->addChildren(getCube(0, 2, 1));
	m_faceTop->addChildren(getCube(0, 2, 2));

	m_faceTop->addChildren(getCube(1, 2, 0));
	m_faceTop->addChildren(getCube(1, 2, 1));
	m_faceTop->addChildren(getCube(1, 2, 2));

	m_faceTop->addChildren(getCube(2, 2, 0));
	m_faceTop->addChildren(getCube(2, 2, 1));
	m_faceTop->addChildren(getCube(2, 2, 2));

	// Back
	m_faceBack = new Object3D();
	m_faceBack->translate(1, 1, 2);

	m_faceBack->addChildren(getCube(0, 0, 2));
	m_faceBack->addChildren(getCube(1, 0, 2));
	m_faceBack->addChildren(getCube(2, 0, 2));

	m_faceBack->addChildren(getCube(0, 1, 2));
	m_faceBack->addChildren(getCube(1, 1, 2));
	m_faceBack->addChildren(getCube(2, 1, 2));

	m_faceBack->addChildren(getCube(0, 2, 2));
	m_faceBack->addChildren(getCube(1, 2, 2));
	m_faceBack->addChildren(getCube(2, 2, 2));

	// Left
	m_faceLeft = new Object3D();
	m_faceLeft->translate(0, 1, 1);

	m_faceLeft->addChildren(getCube(0, 0, 0));
	m_faceLeft->addChildren(getCube(0, 0, 1));
	m_faceLeft->addChildren(getCube(0, 0, 2));

	m_faceLeft->addChildren(getCube(0, 1, 0));
	m_faceLeft->addChildren(getCube(0, 1, 1));
	m_faceLeft->addChildren(getCube(0, 1, 2));

	m_faceLeft->addChildren(getCube(0, 2, 0));
	m_faceLeft->addChildren(getCube(0, 2, 1));
	m_faceLeft->addChildren(getCube(0, 2, 2));

	// Bottom
	m_faceBottom = new Object3D();
	m_faceBottom->translate(1, 0, 1);

	m_faceBottom->addChildren(getCube(0, 0, 0));
	m_faceBottom->addChildren(getCube(0, 0, 1));
	m_faceBottom->addChildren(getCube(0, 0, 2));

	m_faceBottom->addChildren(getCube(1, 0, 0));
	m_faceBottom->addChildren(getCube(1, 0, 1));
	m_faceBottom->addChildren(getCube(1, 0, 2));

	m_faceBottom->addChildren(getCube(2, 0, 0));
	m_faceBottom->addChildren(getCube(2, 0, 1));
	m_faceBottom->addChildren(getCube(2, 0, 2));
}

void RubiksCube::update(float deltaTime)
{
	for (int i=m_asyncCallbacks.size()-1; i >= 0; i--)
	{
		if (m_asyncCallbacks[i](deltaTime))
			m_asyncCallbacks.erase(m_asyncCallbacks.begin() + i);
	}
}

void RubiksCube::rotateFront(Shift dir)
{
	//m_faceFront->rotate(glm::vec3(0, 0, -1), 90);

	int angle = 0;
	m_asyncCallbacks.push_back([&, angle, dir](float deltaTime) mutable
	{
		angle++;

		glm::vec3 axis = (dir == Shift::CW) ? glm::vec3(0, 0, -1) : glm::vec3(0, 0, 1);

		m_faceFront->rotate(axis, 1);

		if (angle > 90)
		{
			angle = 0;
			return true;
		}

		return false;
	});
}

void RubiksCube::rotateRight(Shift dir)
{
	glm::vec3 axis = (dir == Shift::CW) ? glm::vec3(1, 0, 0) : glm::vec3(-1, 0, 0);

	m_faceRight->rotate(axis, 1);
}

void RubiksCube::rotateTop(Shift dir)
{
	glm::vec3 axis = (dir == Shift::CW) ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0);

	m_faceTop->rotate(axis, 1);
}

void RubiksCube::rotateBack(Shift dir)
{
	glm::vec3 axis = (dir == Shift::CW) ? glm::vec3(0, 0, 1) : glm::vec3(0, 0, -1);

	m_faceBack->rotate(axis, 1);
}

void RubiksCube::rotateLeft(Shift dir)
{
	glm::vec3 axis = (dir == Shift::CW) ? glm::vec3(-1, 0, 0) : glm::vec3(1, 0, 0);

	m_faceLeft->rotate(axis, 1);
}

void RubiksCube::rotateBottom(Shift dir)
{
	glm::vec3 axis = (dir == Shift::CW) ? glm::vec3(0, -1, 0) : glm::vec3(0, 1, 0);

	m_faceBottom->rotate(axis, 1);
}

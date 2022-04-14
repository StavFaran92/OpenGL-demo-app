#include "sge.h"

#include "RubiksCube.h"

class RubiksCubeDemo : public Application
{
public:
	using Application::draw;
	Box boxes[3][3][3];

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					boxes[i][j][k] = *createRubiksCubeBox();
					boxes[i][j][k].translate(i, j, k);
				}
			}
		}

		//postProcess(PostProcess::grayscale());
	}

	void draw() override
	{
		if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_X))
		{
			boxes[0][0][0].rotateAround({ 1,1,1 }, {1,0,0}, 1);
			boxes[0][0][1].rotateAround({ 1,1,1 }, {1,0,0}, 1);
			boxes[0][0][2].rotateAround({ 1,1,1 }, {1,0,0}, 1);

			boxes[0][1][0].rotateAround({ 1,1,1 }, {1,0,0}, 1);
			boxes[0][1][1].rotateAround({ 1,1,1 }, {1,0,0}, 1);
			boxes[0][1][2].rotateAround({ 1,1,1 }, {1,0,0}, 1);

			boxes[0][2][0].rotateAround({ 1,1,1 }, {1,0,0}, 1);
			boxes[0][2][1].rotateAround({ 1,1,1 }, {1,0,0}, 1);
			boxes[0][2][2].rotateAround({ 1,1,1 }, {1,0,0}, 1);
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					draw(&boxes[i][j][k]);
				}
			}
		}
	}

};

Application* CreateApplication()
{
	return new RubiksCubeDemo();
}
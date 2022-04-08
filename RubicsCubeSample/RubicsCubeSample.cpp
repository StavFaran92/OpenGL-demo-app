#include "sge.h"

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

Box* createRubicsCubeBox()
{
	return (Box*)ModelBuilder::builder<Box>()
		.getMeshBuilder()
		.setColors(colors, 36)
		.enableAttribute(LayoutAttribute::Colors)
		.disableAttribute(LayoutAttribute::Texcoords)
		.getModelBuilder()
		.build();
}

class RubicsCubeSample : public Application
{
public:
	using Application::draw;
	Box boxes[3][3][3];

	Sphere* sphere;

	Model* guitar;
	Box* box;

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					boxes[i][j][k] = *createRubicsCubeBox();
					boxes[i][j][k].translate(1.1f*i, 1.1f * j, 1.1f * k);
				}
			}
		}

		//postProcess(PostProcess::grayscale());
	}

	void draw() override
	{
		if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_X))
		{
			boxes[0][0][0].rotateX(1);
			boxes[0][0][1].rotateX(1);
			boxes[0][0][2].rotateX(1);

			boxes[0][1][0].rotateX(1);
			boxes[0][1][1].rotateX(1);
			boxes[0][1][2].rotateX(1);

			boxes[0][2][0].rotateX(1);
			boxes[0][2][1].rotateX(1);
			boxes[0][2][2].rotateX(1);
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
	return new RubicsCubeSample();
}
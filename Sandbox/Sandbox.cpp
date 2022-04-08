#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	//Box boxes[3][3][3];

	Sphere* sphere;

	Model* guitar;
	Box* box;

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		//for (int i = 0; i < 3; i++)
		//{
		//	for (int j = 0; j < 3; j++)
		//	{
		//		for (int k = 0; k < 3; k++)
		//		{
		//			boxes[i][j][k] = *createBox();
		//			boxes[i][j][k].translate(1.1f*i, 1.1f * j, 1.1f * k);
		//		}
		//	}
		//}

		float colors[108] = {
		 1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  0.0f,  1.0f,
		 1.0f,  0.0f,  0.0f,

		  1.0f,  0.0f,  0.0f,
		  1.0f,  1.0f,  0.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  0.0f,  1.0f,
		  1.0f,  0.0f,  0.0f,

		  0.0f,  1.0f,  0.0f,
		  0.0f,  1.0f,  1.0f,
		  0.0f,  0.0f,  1.0f,
		  0.0f,  0.0f,  1.0f,
		  0.0f,  0.0f,  0.0f,
		  0.0f,  1.0f,  0.0f,

		  0.0f,  1.0f,  0.0f,
		  0.0f,  1.0f,  1.0f,
		  0.0f,  0.0f,  1.0f,
		  0.0f,  0.0f,  1.0f,
		  0.0f,  0.0f,  0.0f,
		  0.0f,  1.0f,  0.0f,

		  0.0f,  0.0f,  1.0f,
		  0.0f,  1.0f,  1.0f,
		  0.0f,  1.0f,  0.0f,
		  0.0f,  1.0f,  0.0f,
		  0.0f,  0.0f,  0.0f,
		  0.0f,  0.0f,  1.0f,

		  0.0f,  0.0f,  1.0f,
		  0.0f,  1.0f,  1.0f,
		  0.0f,  1.0f,  0.0f,
		  0.0f,  1.0f,  0.0f,
		  0.0f,  0.0f,  0.0f,
		  0.0f,  0.0f,  1.0f
		};

		box = (Box*)ModelBuilder::builder<Box>()
			.getMeshBuilder()
			.setColors(colors, 36)
			.enableAttribute(LayoutAttribute::Colors)
			.getModelBuilder()
			.build();



		//postProcess(PostProcess::grayscale());
	}

	void draw() override
	{
		// Push event to quit
		//SDL_Event e;
		//e.type = SDL_QUIT;
		//SDL_PushEvent(&e);
		
		//addEventListener(SDL_EventType::SDL_MOUSEBUTTONUP, []() {std::cout << "print somethin" << std::endl; });
		//addEventListener(SDL_EventType type, []() {std::cout << "print somethin" << std::endl; });
		//
		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_X))
		//{
		//	boxes[0][0][0].rotateX(1);
		//	boxes[0][0][1].rotateX(1);
		//	boxes[0][0][2].rotateX(1);

		//	boxes[0][1][0].rotateX(1);
		//	boxes[0][1][1].rotateX(1);
		//	boxes[0][1][2].rotateX(1);

		//	boxes[0][2][0].rotateX(1);
		//	boxes[0][2][1].rotateX(1);
		//	boxes[0][2][2].rotateX(1);
		//}

		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Y))
		//{
		//	box->rotateY(1);
		//}

		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Z))
		//{
		//	box->rotateZ(1);
		//}

		draw(box);
		
		//box->rotateY(1);
		//for (int i = 0; i < 3; i++)
		//{
		//	for (int j = 0; j < 3; j++)
		//	{
		//		for (int k = 0; k < 3; k++)
		//		{
		//			draw(&boxes[i][j][k]);
		//		}
		//	}
		//}
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
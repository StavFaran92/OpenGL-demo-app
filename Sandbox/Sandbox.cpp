#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	Box* box;
	Quad* quad;
	Sphere* sphere;

	void start() override
	{
		skybox(Skybox::CreateSkybox());

		box = createBox();
		quad = createQuad();
		quad->translate(0, 0, -2);
		quad->rotateZ(90);

		sphere = Sphere::generateSphere(1, 36, 18);
		sphere->translate(2, 0, 0);

		postProcess(PostProcess::grayscale());
	}

	void draw() override
	{
		// Push event to quit
		//SDL_Event e;
		//e.type = SDL_QUIT;
		//SDL_PushEvent(&e);
		
		//addEventListener(SDL_EventType::SDL_MOUSEBUTTONUP, []() {std::cout << "print somethin" << std::endl; });
		//addEventListener(SDL_EventType type, []() {std::cout << "print somethin" << std::endl; });
		
		if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_X))
		{
			box->rotateX(1);
		}

		if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Y))
		{
			box->rotateY(1);
		}

		if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Z))
		{
			box->rotateZ(1);
		}
		
		//box->rotateY(1);
		draw(box);
		draw(quad);
		draw(sphere);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
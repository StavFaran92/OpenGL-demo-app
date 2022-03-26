#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	Model* box;
	Model* quad;
	Model* sphere;

	void start() override
	{
		skybox(Skybox::CreateSkybox());

		box = createBox();
		quad = createQuad();
		quad->translate(0, 0, -2);
		quad->rotateZ(90);

		sphere = Model::createPrimitiveModel(Model::PrimitiveType::Sphere);
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
		
		if (mouse_middleButtonPressed())
		{
			box->translate(.01, 0, 0);
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
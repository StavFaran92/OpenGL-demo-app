#include "EntryPoint.h"
#include "sge.h"

#include "RubiksCube.h"
#include "RubiksCubeEnt.h"
#include "RubiksCubeConfigurations.h"
#include "RubiksCubeController.h"

class RubiksCubeDemo : public Application
{
public:
	using Application::draw;
	using Application::update;
	std::shared_ptr<RubiksCube> rubiksCube;
	std::shared_ptr<RubiksCubeController> controller;

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		rubiksCube = std::make_shared<RubiksCube>();
		rubiksCube->init(3);

		controller = std::make_shared<RubiksCubeController>();
		controller->init(rubiksCube.get());

		auto scene = Engine::get()->getContext()->getActiveScene();
		scene->enableObjectSelection(true);

		//Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_X, [=](SDL_Event e) 
		//{
		//	std::cout << *a << std::endl;

		//	rubiksCube->rotateFace(Axis::X, 0, Shift::CW);
		//});

		//Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_Y, [&](SDL_Event e)
		//{
		//	rubiksCube->rotateFace(Axis::Y, 0, Shift::CW);
		//});

		//Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_Z, [&](SDL_Event e)
		//{
		//	rubiksCube->rotateFace(Axis::Z, 0, Shift::CW);
		//});

		Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_I, [&](SDL_Event e)
		{
			rubiksCube->print();
		});

		//postProcess(PostProcess::grayscale());
	}

	void update(float deltaTime)
	{
		for (const auto cube : rubiksCube->getCubes())
		{
			update(cube);
		}
	}

	void draw() override
	{
		for (const auto cube : rubiksCube->getCubes())
		{
			draw(cube);
		}
	}

};

Application* CreateApplication()
{
	return new RubiksCubeDemo();
}
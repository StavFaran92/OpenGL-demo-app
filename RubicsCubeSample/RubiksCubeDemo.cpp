#include "EntryPoint.h"
#include "sge.h"

#include "RubiksCube.h"
#include "RubiksCubeEnt.h"
#include "RubiksCubeConfigurations.h"

class RubiksCubeDemo : public Application
{
public:
	using Application::draw;
	using Application::update;
	RubiksCube* rubiksCube;
	Box* box = nullptr;

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		rubiksCube = new RubiksCube();
		rubiksCube->init(5);

		Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_X, [&](SDL_Event e) 
		{
			rubiksCube->rotateFace(Axis::X, 0, Shift::CW);
		});

		Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_Y, [&](SDL_Event e)
		{
			rubiksCube->rotateFace(Axis::Y, 0, Shift::CW);
		});

		Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_Z, [&](SDL_Event e)
		{
			rubiksCube->rotateFace(Axis::Z, 0, Shift::CW);
		});

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
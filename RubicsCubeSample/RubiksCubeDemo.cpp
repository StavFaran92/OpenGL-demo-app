#include "EntryPoint.h"
#include "sge.h"

#include "RubiksCube.h"

class RubiksCubeDemo : public Application
{
public:
	using Application::draw;
	RubiksCube* rubiksCube;

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		rubiksCube = new RubiksCube();
		rubiksCube->init(3);

		Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_Scancode::SDL_SCANCODE_X, [&](SDL_Event e) 
		{
			rubiksCube->rotateFront(RubiksCube::Shift::CW);
		});

		//postProcess(PostProcess::grayscale());
	}

	void update(float deltaTime)
	{
		rubiksCube->update(deltaTime);
	}

	void draw() override
	{
		

		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Z))
		//{
		//	rubiksCube->rotateRight(RubiksCube::Shift::CW);
		//}

		

		//TODO fix
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
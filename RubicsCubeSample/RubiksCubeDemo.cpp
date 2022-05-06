#include "EntryPoint.h"
#include "sge.h"

#include "RubiksCube.h"
#include "RubiksCubeEnt.h"
#include "RubiksCubeConfigurations.h"
#include "RubiksCubeController.h"
#include "RubiksCubeGui.h"

class RubiksCubeDemo : public Application
{
public:
	using Application::draw;
	using Application::update;
	std::shared_ptr<RubiksCube> rubiksCube;
	std::shared_ptr<RubiksCubeController> controller;
	std::shared_ptr<RubiksCubeGui> gui;

	void start() override
	{
		//skybox(Skybox::CreateSkybox());

		auto camera = std::dynamic_pointer_cast<EditorCamera>(Engine::get()->getContext()->getActiveScene()->getRenderer()->getCamera());
		camera->lookAt(1, 1, 1);
		camera->setPosition(13, 225, 35);

		rubiksCube = std::make_shared<RubiksCube>();
		rubiksCube->init(3);

		controller = std::make_shared<RubiksCubeController>();
		controller->init(rubiksCube.get());

		gui = std::make_shared<RubiksCubeGui>(rubiksCube.get());

		auto scene = Engine::get()->getContext()->getActiveScene();
		scene->enableObjectSelection(true);

		//postProcess(PostProcess::grayscale());
	}

	void update(float deltaTime)
	{
		for (const auto cube : rubiksCube->getCubes())
		{
			update(cube.object());
		}
	}

	void draw() override
	{
		for (const auto cube : rubiksCube->getCubes())
		{
			draw(cube.object());
		}

		Engine::get()->getImguiHandler()->draw(gui.get());
	}

};

Application* CreateApplication()
{
	return new RubiksCubeDemo();
}
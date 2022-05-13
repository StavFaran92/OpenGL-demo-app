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

	ObjectHandler<Skybox> skybox;

	void start() override
	{
		skybox = Skybox::CreateSkybox();

		rubiksCube = std::make_shared<RubiksCube>();
		rubiksCube->init(7);

		controller = std::make_shared<RubiksCubeController>();
		controller->init(rubiksCube.get());

		auto camera = std::dynamic_pointer_cast<EditorCamera>(Engine::get()->getContext()->getActiveScene()->getRenderer()->getCamera());
		float center = (rubiksCube->getSize() - 1) / 2;
		camera->lookAt(center, center, center);
		camera->setPosition(13, 225, 35);

		gui = std::make_shared<RubiksCubeGui>(rubiksCube.get());

		auto scene = Engine::get()->getContext()->getActiveScene();
		scene->enableObjectSelection(true);

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

		Engine::get()->getImguiHandler()->draw(gui.get());

		Engine::get()->getContext()->getActiveScene()->drawSkybox(skybox);
	}

	void close() override
	{
		skybox.release();
	}

};

Application* CreateApplication()
{
	return new RubiksCubeDemo();
}
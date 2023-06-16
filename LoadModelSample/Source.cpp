#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;

	void start() override
	{
		auto importer = getContext()->getModelImporter();
		auto guitar = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());
	}

	void update(float deltaTime) override
	{ }

	void draw() override
	{ }

};

Application* CreateApplication()
{
	return new Sandbox();
}
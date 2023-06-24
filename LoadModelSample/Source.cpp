#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:
	void start() override
	{
		auto importer = getContext()->getModelImporter();
		auto guitar = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
#include "EntryPoint.h"
#include "sge.h"

class CustomBoxBehaviour : public ScriptableEntity
{
	virtual void onCreate() override
	{
		std::cout << "Box was created modafaka.\n";
	}
};

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;

	void start() override
	{
		auto box1 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		auto& nsc = box1->addComponent<NativeScriptComponent>();
		nsc.bind<CustomBoxBehaviour>();
	}

	void update(float deltaTime) override
	{
	}

	void draw() override
	{
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
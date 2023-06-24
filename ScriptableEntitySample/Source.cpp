#include "EntryPoint.h"
#include "sge.h"

class CustomBoxBehaviour : public ScriptableEntity
{
	virtual void onUpdate(float a) override
	{
		std::cout << "Box was created modafaka.\n";
	}
};

class Sandbox : public Application
{
public:
	void start() override
	{
		auto box1 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		auto& nsc = box1->addComponent<NativeScriptComponent>();
		nsc.bind<CustomBoxBehaviour>();
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
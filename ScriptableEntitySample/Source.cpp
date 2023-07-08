#include "EntryPoint.h"
#include "sge.h"

class CustomBoxBehaviour : public ScriptableEntity
{
	virtual void onUpdate(float a) override
	{
		std::cout << "Box was created modafaka.\n";
	}
};

class CustomBoxBehaviourWithParams : public ScriptableEntity
{
public:
	CustomBoxBehaviourWithParams(float a) : m_a(a){}

	virtual void onUpdate(float a) override
	{

	}

	float m_a;
};

class Sandbox : public Application
{
public:
	void start() override
	{
		//{
		//	auto box1 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		//	auto& nsc = box1.addComponent<NativeScriptComponent>();
		//	nsc.bind<CustomBoxBehaviour>();
		//}

		{
			auto box2 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			NativeScriptComponent nsc;
			auto cbb = box2.addComponent<NativeScriptComponent>(&nsc).bind<CustomBoxBehaviourWithParams>(5.0f);
			std::cout << std::to_string(cbb->m_a) << std::endl;
		}
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
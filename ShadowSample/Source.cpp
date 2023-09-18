#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:

	void start() override
	{
		auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());

		auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		box.getComponent<Transformation>().translate({0, 1, 0});

		auto ground = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		auto& groundTransfrom = ground.getComponent<Transformation>();
		groundTransfrom.setLocalScale({ 50, .5f, 50 });

		//postProcess(PostProcess::grayscale());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
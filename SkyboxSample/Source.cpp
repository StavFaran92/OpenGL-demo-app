#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;

	void start() override
	{
		auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());




		//postProcess(PostProcess::grayscale());
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
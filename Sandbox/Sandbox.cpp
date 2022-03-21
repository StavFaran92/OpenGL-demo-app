#include "sge.h"

class Sandbox : public Application
{
public:

	Window* window;
	Model* box;

	void start() override
	{
		auto context = Engine::get()->getContext();

		context->getActiveScene()->setSkybox(Skybox::CreateSkybox());

		box = createBox();
		context->getActiveScene()->addModel(box);

	}

	void draw() override
	{
		box->translate(.001, 0, 0);
		box->rotateX(30);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
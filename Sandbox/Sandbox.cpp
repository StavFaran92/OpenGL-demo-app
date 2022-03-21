#include "sge.h"

class Sandbox : public Application
{
public:

	Window* window;
	Model* box;

	void start() override
	{
		auto context = Engine::get()->getContext();

		context->getActiveScene()->setSkybox(std::shared_ptr<Skybox>(Skybox::CreateSkybox()));

		//box = Primitives::createBox();
		//box->GetTransformation()->Translate(100, 0, 0);
		//box->GetTransformation()->rotateX(30);
	}

	void draw() override
	{
		//draw(box);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
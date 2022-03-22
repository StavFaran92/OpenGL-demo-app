#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	Model* box;
	Model* quad;

	void start() override
	{
		skybox(Skybox::CreateSkybox());

		box = createBox();
		quad = createQuad();
		quad->translate(0, 0, -2);
		quad->rotateZ(90);

		postProcess(PostProcess::grayscale());
	}

	void draw() override
	{
		box->translate(.001, 0, 0);
		box->rotateY(1);
		draw(box);
		draw(quad);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
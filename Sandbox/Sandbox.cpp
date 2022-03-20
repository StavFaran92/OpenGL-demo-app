#include "sge.h"

class Sandbox : public Application
{
public:

	Window* window;
	Model* box;

	void start() override
	{
		//window = Window::createWindow(600, 360);


		//box = Primitives::createBox();
		//box->translate(100, 0, 0);
		//box->rotateX(30);
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
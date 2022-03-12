#include "GraphicsEngine.h"

class Sandbox : public Application
{
public:
	Sandbox() {}

	~Sandbox() {}




	// Inherited via Application
	virtual void start() override
	{
	}

	virtual void update() override
	{
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
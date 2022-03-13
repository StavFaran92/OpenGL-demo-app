#include "GraphicsEngine.h"

class Sandbox : public Application
{
public:
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
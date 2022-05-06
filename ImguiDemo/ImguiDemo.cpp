#include "EntryPoint.h"
#include "sge.h"

#include "MockMenu.h"

class ImguiDemo : public Application
{
public:
	using Application::draw;
	using Application::update;

	std::shared_ptr<MockMenu> menu;

	void start() override
	{
		menu = std::make_shared<MockMenu>();
	}

	void update(float deltaTime)
	{

	}

	void draw() override
	{
		Engine::get()->getImguiHandler()->draw(menu.get());
	}

};

Application* CreateApplication()
{
	return new ImguiDemo();
}
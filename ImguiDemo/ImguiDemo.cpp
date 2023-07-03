#include "EntryPoint.h"
#include "sge.h"

#include "MockMenu.h"

class ImguiDemo : public Application
{
public:
	std::shared_ptr<MockMenu> menu;

	void start() override
	{
		menu = std::make_shared<MockMenu>();
	}
};

Application* CreateApplication()
{
	return new ImguiDemo();
}
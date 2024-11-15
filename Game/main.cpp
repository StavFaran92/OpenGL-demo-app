#include "EntryPoint.h"
#include "sge.h"

class Game : public Application
{
public:

	void start() override
	{

	}

};

Application* CreateApplication()
{
	return new Game();
}
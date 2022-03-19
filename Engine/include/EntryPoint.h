#pragma once
#include "sge\Engine.h"
#include "Application.h"

Application* CreateApplication();

int main()
{ 
	auto engine = Engine::get();
	engine->init();

	auto app = CreateApplication();
	app->start();

	engine->run(app);

	app->close();
	engine->close();

	return 0;
}
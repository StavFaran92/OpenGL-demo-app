#pragma once
#include "Engine.h"
#include "Application.h"

Application* CreateApplication();

void sgeEntry()
{
	auto engine = Engine::get();
	engine->init();

	auto app = CreateApplication();
	app->start();

	engine->run(app);

	app->close();
	engine->close();
}

int main()
{ 
	sgeEntry();

	return 0;
}
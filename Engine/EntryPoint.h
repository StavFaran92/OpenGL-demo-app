#pragma once
#include "Engine.h"

Application* CreateApplication();

int main(int argc, char* argv[])
{ 
	auto engine = Engine::Get();
	engine->Init();

	auto app = CreateApplication();
	app->start();

	engine->Run(app);

	app->close();
	engine->Close();

	return 0;
}
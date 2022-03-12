#pragma once
#include "Engine.h"
#include "Application.h"

Application* createApplication();

int main(int argc, char* argv[])
{ 
	auto engine = Engine::Get();
	engine->Init();

	auto app = createApplication();
	app->start();

	//auto context = engine->GetContext();

	//glEnable(GL_CULL_FACE);

	//auto grid = Grid::GenerateGrid(10);
	//context->AddModel(grid);

	//auto skybox = Skybox::CreateSkybox();
	//context->AddSkybox(skybox);

	engine->Run(app);

	engine->Close();

	return 0;
}
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <filesystem>

#include <GL/glew.h>

#include "Utils/Math/LinearAlgebraUtil.h"

#include "Window.h"

#include "Graphics/Geometry/Mesh.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Camera/FlyCamera.h"
#include "Graphics/Shader/Texture.h"
#include "Graphics/Lighting/DirectionalLight.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Shader/Material.h"

#include "Graphics/Buffers/FrameBufferObject.h"
#include "Graphics/Buffers/RenderBufferObject.h"

#include "Graphics/Services/ScreenBufferProjector.h"

#include "main.h"

#include "Graphics/Geometry/Model.h"

#include "Utils/Logger/Logger.h"
#include "spdlog/spdlog.h"

#include "imgui/ImguiHandler.h"

#include "Core/Application.h"
#include "Core/Context.h"

#include "Graphics/Models/Grid.h"

void handleKeys(unsigned char key, int x, int y);
void handleEvents(SDL_Event& e, std::shared_ptr<ImguiHandler> imgui, bool& quit, std::shared_ptr<ICamera> camera, double deltaTime);

int main(int argc, char* argv[])
{ 
	Application::Get().Init();

	std::shared_ptr<Window> mainWindow = Application::Get().GetWindow();
	std::shared_ptr<Context> context = Application::Get().GetContext();
	std::shared_ptr<Renderer> renderer = Application::Get().GetRenderer();
	std::shared_ptr<ImguiHandler> imgui = Application::Get().GetImguiHandler();
	std::shared_ptr<ScreenBufferProjector> screenProjector = Application::Get().GetScreenBufferProjector();

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	//auto grid = Grid::GenerateGrid(10);
	//context->AddModel(grid);

	renderer->Clear();
	float angle = 0;
	// Loop until window closed
	while (!quit)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)SDL_GetPerformanceFrequency() / ((NOW - LAST) * 1000);

		//Handle events on queue
		handleEvents(e, imgui, quit, renderer->GetCamera(), deltaTime);

		renderer->Clear();

		renderer->GetCamera()->update(deltaTime);

		//lightCube->GetTransformation()->SetPosition({ 10 * cos(angle * Constants::toRadians) ,0, 10 * sin(angle * Constants::toRadians) });
		//angle++;
		//lightCube->GetTransformation()->SetScale({ .25f, .25f, .25f });

		context->Update(deltaTime);

		screenProjector->RedirectToFrameBuffer();

		context->Draw();

		screenProjector->RedirectToDefault();

		imgui->Render();

		mainWindow->SwapBuffer();
	}

	imgui->Close();

	mainWindow->Close();

	return 0;
}

void handleEvents(SDL_Event& e, std::shared_ptr<ImguiHandler> imgui, bool& quit, std::shared_ptr<ICamera> camera, double deltaTime)
{
	while (SDL_PollEvent(&e) != 0)
	{
		imgui->ProccessEvents(e);

		//User requests quit
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		//Handle keypress with current mouse position
		else if (e.type == SDL_MOUSEMOTION)
		{
			camera->OnMouseMotion(e.motion.xrel, e.motion.yrel);
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			camera->OnMousePressed(e.button);
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			camera->OnMouseReleased(e.button);
		}
		else if (e.type == SDL_MOUSEWHEEL)
		{
			camera->OnMouseScroll(e.wheel.y);
		}
	}
}
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <filesystem>

#include <GL/glew.h>

#include "Utils/Math/LinearAlgebraUtil.h"

#include "Window.h"

#include "Renderer/Geometry/Mesh.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Camera/FlyCamera.h"
#include "Renderer/Shader/Texture.h"
#include "Renderer/Lighting/DirectionalLight.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader/Material.h"

#include "main.h"

#include "Renderer/Geometry/Model.h"

#include "Utils/Logger/Logger.h"
#include "spdlog/spdlog.h"

#include "imgui/ImguiHandler.h"

#include "Core/Application.h"

void handleKeys(unsigned char key, int x, int y);
void handleEvents(SDL_Event& e, ImguiHandler& imgui, bool& quit, std::shared_ptr<ICamera> camera, double deltaTime);

int main(int argc, char* argv[])
{ 
	Application::Get().Init();

	std::shared_ptr<Window> mainWindow = Application::Get().GetWindow();
	std::shared_ptr<Context> context = Application::Get().GetContext();

	ImguiHandler imgui;
	if (!imgui.Init(mainWindow->GetWindow(), mainWindow->GetContext()))
	{
		logError("Imgui init failed!");
		return -1;
	}

	Renderer renderer;

	Shader modelShader("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag");
	Shader lightShader("Resources\\Shaders\\LightShader.vert", "Resources\\Shaders\\LightShader.frag");

	std::shared_ptr<ICamera> camera = std::make_shared<FlyCamera>(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 1.0f, .5f);

	glm::mat4 projection = glm::perspective(45.0f, (float)mainWindow->getWidth() / mainWindow->getHeight(), 0.1f, 100.0f);

	Model lightCube("D:\\program files\\downloads\\cube-companion\\source\\model\\model.dae");
	lightCube.loadModel();

	Model backpack("D:\\program files\\downloads\\backpack\\backpack.obj");
	backpack.loadModel();

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float angle = 0;
	// Loop until window closed
	while (!quit)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)SDL_GetPerformanceFrequency() / ((NOW - LAST) * 1000);

		//Handle events on queue
		handleEvents(e, imgui, quit, camera, deltaTime);

		renderer.Clear();

		camera->update(deltaTime);

		lightShader.UseShader();

		lightShader.SetMat4("projection", projection);
		lightShader.SetMat4("view", camera->getView());

		lightCube.GetTransformation()->SetPosition({ 10 * cos(angle * Constants::toRadians) ,0, 10 * sin(angle * Constants::toRadians) });
		angle++;
		lightCube.GetTransformation()->SetScale({ .25f, .25f, .25f });
		lightCube.Update(deltaTime);
		lightCube.Draw(lightShader, renderer);

		modelShader.UseShader();

		modelShader.SetFloat("material.shininess", 32.0f);

		modelShader.SetFloat("light.color", { 1.0f, 1.0f, 1.0f });
		modelShader.SetFloat("light.position", lightCube.GetTransformation()->GetPosition());
		modelShader.SetFloat("light.ambient", { 0.2f, 0.2f, 0.2f });
		modelShader.SetFloat("light.diffuse", { 0.5f, 0.5f, 0.5f }); // darken diffuse light a bit
		modelShader.SetFloat("light.specular", { 1.0f, 1.0f, 1.0f });

		modelShader.SetFloat("viewPos", camera->getPosition());

		modelShader.SetMat4("projection", projection);
		modelShader.SetMat4("view", camera->getView());

		backpack.GetTransformation()->SetPosition({ 0,0,0 });
		backpack.Update(deltaTime);
		backpack.Draw(modelShader, renderer);

		imgui.Render();

		mainWindow->SwapBuffer();
	}

	imgui.Close();

	mainWindow->Close();

	return 0;
}

void handleEvents(SDL_Event& e, ImguiHandler& imgui, bool& quit, std::shared_ptr<ICamera> camera, double deltaTime)
{
	while (SDL_PollEvent(&e) != 0)
	{
		imgui.ProccessEvents(e);

		//User requests quit
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		//Handle keypress with current mouse position
		else if (e.type == SDL_MOUSEMOTION)
		{
			camera->mouseControl(e.motion.xrel, e.motion.yrel);
		}
	}
}
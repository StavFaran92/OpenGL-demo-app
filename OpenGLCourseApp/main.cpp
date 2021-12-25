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
#include "Renderer/Camera/Camera.h"
#include "Renderer/Shader/Texture.h"
#include "Renderer/Lighting/DirectionalLight.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader/Material.h"

#include "Vendor/imgui/imgui.h"
//#include "Vendor/imgui/imgui_impl_glfw_gl3.h"

#include "Tests/Test.h"
#include "Tests/TestMenu.h"
#include "Tests/TestClearColor.h"
#include "main.h"

#include "Renderer/Geometry/Model.h"

void handleKeys(unsigned char key, int x, int y);

const float toRadians = 3.1315265f / 180;

DirectionalLight mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Material shinyMaterial;
Material dullMaterial;

int main(int argc, char* argv[])
{
	Window mainWindow(800, 600);
	mainWindow.initialize();

	Renderer renderer;

	Shader modelShader("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag");
	Shader lightShader("Resources\\Shaders\\LightShader.vert", "Resources\\Shaders\\LightShader.frag");

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 1.0f, .5f);

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
		0.3f, 2.0f, 
		0.0f, 0.0f, -1.0f);

	glm::mat4 projection = glm::perspective(45.0f, (float)mainWindow.getWidth() / mainWindow.getHeight(), 0.1f, 100.0f);

	Model lightCube("D:\\program files\\downloads\\cube-companion\\source\\model\\model.dae");
	lightCube.loadModel();

	Model backpack("D:\\program files\\downloads\\source\\model.obj");
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
		handleEvents(e, quit, camera, deltaTime);

		renderer.Clear();
		
		camera.update(deltaTime);

		lightShader.UseShader();

		lightShader.SetMat4("projection", projection);
		lightShader.SetMat4("view", camera.getView());

		lightCube.GetTransformation()->SetPosition({ 5 * cos(angle * toRadians) ,0, 5*sin(angle * toRadians) });
		angle++;
		lightCube.GetTransformation()->SetScale({ .25f, .25f, .25f });
		lightCube.Update(deltaTime);
		lightCube.Draw(modelShader, renderer);

		modelShader.UseShader();

		modelShader.SetFloat("lightColor", { 1.0f, 1.0f, 1.0f });
		modelShader.SetFloat("lightPos", lightCube.GetTransformation()->GetPosition());

		modelShader.SetFloat("material.ambient", {1.0f, 0.5f, 0.31f});
		modelShader.SetFloat("material.diffuse", {1.0f, 0.5f, 0.31f});
		modelShader.SetFloat("material.specular",{ 0.5f, 0.5f, 0.5f});
		modelShader.SetFloat("material.shininess", 32.0f);

		modelShader.SetFloat("viewPos", camera.getPosition());
		//mainLight.useLight(modelShader);
		//shinyMaterial.UseMaterial(modelShader);

		modelShader.SetMat4("projection", projection);
		modelShader.SetMat4("view", camera.getView());
		//modelShader.SetFloat("eyePosition", camera.getPosition());

		backpack.GetTransformation()->SetPosition({0,0,0});
		backpack.Update(deltaTime);
		backpack.Draw(modelShader, renderer);


		mainWindow.SwapBuffer();
	}

	mainWindow.Close();

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

void handleEvents(SDL_Event& e, bool& quit, Camera& camera, double deltaTime)
{
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		//Handle keypress with current mouse position
		else if (e.type == SDL_MOUSEMOTION)
		{
			camera.mouseControl(e.motion.xrel, e.motion.yrel);

		}
	}
}


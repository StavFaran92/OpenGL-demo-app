#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

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

void handleKeys(unsigned char key, int x, int y);

const float toRadians = 3.1315265f / 180;

std::vector<Mesh*> meshList;

DirectionalLight mainLight;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Material shinyMaterial;
Material dullMaterial;

float curAngle = 0;

void CreateObject(const Shader& shader, const Renderer& renderer)
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	
	GLfloat vertices[] = {
	//	x		y		z		u	  v  		nx,	  ny,   nz
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	calcAverageNormal(indices, 12, vertices, 32, 8, 5);

	Mesh *obj = new Mesh(shader, renderer);
	obj->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj);
}

int main(int argc, char* argv[])
{
	Window mainWindow(800, 600);
	mainWindow.initialize();

	Renderer renderer;

	Shader shader("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag");

	CreateObject(shader, renderer);

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, .5f);

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(.3f, 4);

	Texture brickTexture("Resources\\Textures\\brick.png");
	brickTexture.LoadTexture();
	Texture dirtTexture("Resources\\Textures\\dirt.png");
	dirtTexture.LoadTexture();

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
		0.3f, 2.0f, 
		0.0f, 0.0f, -1.0f);

	glm::mat4 projection = glm::perspective(45.0f, (float)mainWindow.getWidth() / mainWindow.getHeight(), 0.1f, 100.0f);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glm::vec3 translation(0.0f, 0.0f, -2.5f);

	glm::mat4 model(1.0f);
	model = glm::translate(model, translation);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Loop until window closed
	while (!quit)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)SDL_GetPerformanceFrequency() / ((NOW - LAST) * 1000);

		//Handle events on queue
		handleEvents(e, quit, camera, deltaTime);

		renderer.Clear();

		shader.UseShader();

		mainLight.useLight(shader);
		shinyMaterial.UseMaterial(shader);

		shader.SetMat4("model", model);
		shader.SetMat4("projection", projection);
		shader.SetMat4("view", camera.calculateViewMatrix());
		shader.SetFloat("eyePosition", camera.getCameraPosition());

		brickTexture.Bind();

		meshList[0]->RenderMesh();

		camera.keyControl(deltaTime);

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
		if (e.type == SDL_KEYDOWN)
		{
			
		}
	}
}


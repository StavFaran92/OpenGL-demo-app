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

void handleKeys(unsigned char key, int x, int y);

const float toRadians = 3.1315265f / 180;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

Texture brickTexture;
Texture dirtTexture;

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

	//ImGui::CreateContext();
	//ImGui_ImplGlfwGL3_Init(mainWindow.GetWindow(), true);
	//ImGui::StyleColorsDark();

	Shader shader("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag");
	shaderList.push_back(&shader);

	CreateObject(shader, renderer);

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(.3f, 4);

	brickTexture = Texture("Resources\\Textures\\brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Resources\\Textures\\dirt.png");
	dirtTexture.LoadTexture();

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
		0.3f, 2.0f, 
		0.0f, 0.0f, -1.0f);

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition, uniformAmbientColor = 0, uniformAmbientIntensity = 0,
		uniformDirection = 0, uniformDiffuseIntensity = 0, uniformSpecularIntensity, uniformShininess;
	std::cout << "buffer width: " + mainWindow.getBufferWidth() << std::endl;
	std::cout << "buffer height: " + mainWindow.getBufferHeight() << std::endl;
	glm::mat4 projection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);

	bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glm::vec3 translation(0.0f, 0.0f, -2.5f);

	//test::Test* currentTest = nullptr;
	//test::TestMenu* menu = new test::TestMenu(currentTest);
	//currentTest = menu;

	//menu->RegisterTest<test::TestClearColor>("Clear Color");

			//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	// Loop until window closed
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//Handle keypress with current mouse position
			else if (e.type == SDL_TEXTINPUT)
			{
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
			}
		}

		//camera.keyControl(mainWindow.getKeys(), deltaTime);
		//camera.mouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		renderer.Clear();



		uniformModel = shaderList[0]->GetUniformLocation("model");
		uniformProjection = shaderList[0]->GetUniformLocation("projection");
		uniformView = shaderList[0]->GetUniformLocation("view");
		uniformAmbientColor = shaderList[0]->GetUniformLocation("directionalLight.color");
		uniformAmbientIntensity = shaderList[0]->GetUniformLocation("directionalLight.ambientIntensity");
		uniformDirection = shaderList[0]->GetUniformLocation("directionalLight.direction");
		uniformDiffuseIntensity = shaderList[0]->GetUniformLocation("directionalLight.diffuseIntensity");
		uniformSpecularIntensity = shaderList[0]->GetUniformLocation("material.specularIntensity");
		uniformShininess = shaderList[0]->GetUniformLocation("material.shininess");
		uniformEyePosition = shaderList[0]->GetUniformLocation("eyePosition");

		mainLight.useLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

		glm::mat4 model(1.0f);
		model = glm::translate(model, translation);
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		brickTexture.Bind();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[0]->RenderMesh();

		//{
		//	ImGui::SliderFloat2("translation", &translation.x, -1.0f, 1.0f);
		//}

		//ImGui::Render();
		//ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		mainWindow.SwapBuffer();
	}

	//ImGui_ImplGlfwGL3_Shutdown();
	//ImGui::DestroyContext();

	return 0;
}


#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;


	ObjectHandler<Model> plane;


	void start() override
	{
		auto importer = getContext()->getModelImporter();
		plane = importer->loadModelFromFile("C:/Users/Stav/Documents/blender/plane_v2.fbx");

		plane.object()->rotate({1,0,0}, 90);
		plane.object()->rotate({0,1,0}, 90);

		Shader* shader = new Shader("Resources/Shaders/OceanVertexShader.vert", "Resources/Shaders/OceanFragmentShader.frag");
		//shader->setFloat("amplitude", 1);
		//shader->setFloat("waveDirection", glm::vec2(1,1));
		//shader->setFloat("waveLength", 128);
		//shader->setFloat("waveSpeed", 10);
		plane.object()->attachShader(shader);
	}

	void update(float deltaTime) override
	{
		update(plane);
		//update(guitar);
		//update(sphere1);
		//update(sphere2);
	}

	void draw() override
	{
		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_X))
		//{
		//	box->rotate({0,0,1}, 1);
		//}

		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Z))
		//{
		//	sphere1->rotate({ 0,0,1 }, 1);
		//}

		getContext()->getActiveScene()->draw(plane);

		//draw(box);
		//draw(guitar);
		//draw(sphere1);
		//draw(sphere2);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
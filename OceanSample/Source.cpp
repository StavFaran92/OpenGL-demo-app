#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;


	ObjectHandler<Model> plane;
	ObjectHandler<Model> light;


	void start() override
	{
		auto importer = getContext()->getModelImporter();
		plane = importer->loadModelFromFile("C:/Users/Stav/Documents/blender/plane_v2.fbx");

		plane.object()->rotate({1,0,0}, 90);
		plane.object()->rotate({0,1,0}, 90);

		StandardShader* shader = new StandardShader("Resources/Shaders/OceanVertexShader.vert", "Resources/Shaders/OceanFragmentShader.frag");
		shader->SetEnableLights(true);
		shader->SetEnableMaterials(true);
		shader->SetEnableTextures(true);
		//shader->setFloat("amplitude", 1);
		//shader->setFloat("waveDirection", glm::vec2(1,1));
		//shader->setFloat("waveLength", 128);
		//shader->setFloat("waveSpeed", 10);
		plane.object()->attachShader(shader);

		auto dLight = new PointLight(glm::vec3{ 1,3,1 }, glm::vec3{ 0,1,-1 }, 1, 1, Attenuation());
		getContext()->getActiveScene()->addPointLight(dLight);

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
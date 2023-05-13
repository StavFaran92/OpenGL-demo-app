#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;


	ObjectHandler<Model> guitar;
	ObjectHandler<Quad> quad;
	//Transformation trans1{ { 5,5,5 } };
	//Transformation trans2{ { 2,2,2 } };


	std::vector<Transformation> transformations;


	void start() override
	{
		quad = ModelBuilder::builder<Quad>()
			.build();

		//auto importer = getContext()->getModelImporter();
		//guitar = importer->loadModelFromFile("D:\\program files\\downloads\\backpack\\backpack.obj");

		//transformations.push_back(&trans1);
		//transformations.push_back(&trans2);
	}

	void update(float deltaTime) override
	{
		//update(box);
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

		getContext()->getActiveScene()->draw(quad);

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
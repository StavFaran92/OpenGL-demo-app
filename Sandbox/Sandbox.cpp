#include "EntryPoint.h"
#include "sge.h"

float colors[108] = {
	//Red
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	//Green
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,

	//Blue
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,

	//Yellow
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  0.0f,

	//Purple
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,
	1.0f,  0.0f,  1.0f,

	//White
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
};

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;


	ObjectHandler<Model> guitar;
	ObjectHandler<Box> box;
	Sphere* sphere1;
	Sphere* sphere2;
	ObjectHandler<Skybox> skybox;
	Transformation trans1{ { 1,1,1 } };
	Transformation trans2{ { 2,2,2 } };

	
	std::vector<Transformation*> transformations;


	void start() override
	{
		skybox = Skybox::CreateSkybox();

		box = ModelBuilder::builder<Box>()
			.build();

		auto importer = getContext()->getModelImporter();
		guitar = importer->loadModelFromFile("D:\\program files\\downloads\\backpack\\backpack.obj");

		transformations.push_back(&trans1);
		transformations.push_back(&trans2);

		

		//sphere1 = (Sphere*)ModelBuilder::builder<Sphere>(1, 36, 18)
		//	.build();

		//sphere2 = (Sphere*)ModelBuilder::builder<Sphere>(1, 36, 18)
		//	.build();

		//sphere1->translate(3, 0, 0);
		//sphere2->translate(6, 0, 0);

		//box->addChildren(sphere1);
		//sphere1->addChildren(sphere2);

		//postProcess(PostProcess::grayscale());
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

		getContext()->getActiveScene()->drawMultiple(guitar, &transformations);

		//draw(box);
		//draw(guitar);
		//draw(sphere1);
		//draw(sphere2);
		getContext()->getActiveScene()->drawSkybox(skybox);

	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
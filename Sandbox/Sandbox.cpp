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
	//ObjectHandler<Box> box;
	Sphere* sphere1;
	Sphere* sphere2;
	//ObjectHandler<Skybox> skybox;
	//Transformation trans1{ { 5,5,5 } };
	//Transformation trans2{ { 2,2,2 } };
	
	//std::shared_ptr<Entity>skybox;
	//std::shared_ptr<Entity>box;
	std::vector<Transformation> transformations;
	

	void start() override
	{
		// If i remove this the skybox works
		//auto box1 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		//auto box2 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());

		//box1->getComponent<Transformation>().translate({1, 0,0});
		//auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());


		auto importer = getContext()->getModelImporter();
		auto guitar = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());

		//auto& transform = guitar->getComponent<Transformation>();
		//transform.translate({ 10, 0,0 });

		//transformations.push_back(&trans1);
		//transformations.push_back(&trans2);

		const int gridLength = 100;

		for (int i = 0; i < gridLength; i++)
		{
			for (int j = 0; j < gridLength; j++)
			{
				Transformation trans({(i - gridLength / 2) * 2, 0, (j - gridLength / 2) * 2});
				transformations.push_back(trans);
			}
		}

		//box1->addComponent<InstanceBatch>( transformations );
		//guitar->RemoveComponent<RenderableComponent>( );

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

		//getContext()->getActiveScene()->drawMultiple({box, transformations});

		////draw(box);
		////draw(guitar);
		////draw(sphere1);
		////draw(sphere2);
		//getContext()->getActiveScene()->drawSkybox(skybox);

	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
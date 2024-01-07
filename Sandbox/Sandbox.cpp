#include "EntryPoint.h"
#include "sge.h"

class CustomBoxBehaviour : public ScriptableEntity
{
	virtual void onCreate() override
	{
		std::cout << "Box was created modafaka.\n";
	}
};

class Sandbox : public Application
{
public:

	void start() override
	{
		//auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());

		//auto box1 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		//auto& nsc = box1.addComponent<NativeScriptComponent>();
		//nsc.bind<CustomBoxBehaviour>();
		//box1->getComponent<Transformation>().translate({1, 0,0});
		//auto box2 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());


		auto importer = getContext()->getModelImporter();
		auto guitar = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());

		//auto& transform = guitar->getComponent<Transformation>();
		//transform.translate({ 10, 0,0 });

		//transformations.push_back(&trans1);
		//transformations.push_back(&trans2);

		//const int gridLength = 100;

		//for (int i = 0; i < gridLength; i++)
		//{
		//	for (int j = 0; j < gridLength; j++)
		//	{
		//		Transformation trans({(i - gridLength / 2) * 2, 0, (j - gridLength / 2) * 2});
		//		transformations.push_back(trans);
		//	}
		//}

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

};

Application* CreateApplication()
{
	return new Sandbox();
}
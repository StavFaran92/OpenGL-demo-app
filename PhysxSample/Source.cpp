#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:

	void start() override
	{
		Skybox::CreateSkybox();

		auto camera = Engine::get()->getContext()->getActiveScene()->getActiveCamera();
		camera->lookAt(0, 5, 0);
		camera->setPosition(25, 225, 35);

		//{
		//	auto importer = getContext()->getModelImporter();
		//	auto guitar = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());

		//	auto& boxTransform = guitar.getComponent<Transformation>();
		//	boxTransform.setLocalPosition({ 0, 10, .5f });
		//	boxTransform.rotate({ 0, 1, 0 }, 45);
		//	//boxTransform.rotate({ 0, 0, 1 }, 180);

		//	auto& rb = guitar.addComponent<RigidBodyComponent>();
		//	rb.mass = 1;
		//	rb.type = RigidbodyType::Dynamic;

		//	guitar.addComponent<CollisionMeshComponent>();

		//	for (auto& child : guitar.getChildren())
		//	{
		//		child.second.addComponent<CollisionMeshComponent>();

		//		//auto& rb = child.second.addComponent<RigidBodyComponent>();
		//		//rb.mass = 1;
		//		//rb.type = RigidbodyType::Dynamic;
		//	}
		//}


		auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		{

			auto& boxTransform = box.getComponent<Transformation>();
			boxTransform.setLocalPosition({ 0, 10, .5f });
			boxTransform.rotate({ 0, 1, 0 }, 45);
			//boxTransform.rotate({ 0, 0, 1 }, 180);

			auto& rb = box.addComponent<RigidBodyComponent>();
			rb.mass = 1;
			rb.type = RigidbodyType::Dynamic;

			auto& collisionBox = box.addComponent<CollisionBoxComponent>();
			collisionBox.halfExtent = .5f;
		}

		//for(int i=0; i<100; i++)
		//	for (int j = 0; j < 100; j++)
		//		{
		//			auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());

		//			auto& boxTransform = box.getComponent<Transformation>();
		//			boxTransform.setPosition({ i, 12, j });
		//			//boxTransform.rotate({ 0, 1, 0 }, 90);
		//			boxTransform.rotate({ 0, 0, 1 }, 50);

		//			//auto& rb = box.addComponent<RigidBodyComponent>();
		//			//rb.mass = 1;
		//			//rb.type = RigidbodyType::Dynamic;

		//			//auto& collisionBox = box.addComponent<CollisionBoxComponent>();
		//			//collisionBox.halfExtent = .5f;
		//		}

		auto sphere = ShapeFactory::createSphere(Engine::get()->getContext()->getActiveScene().get());
		{

			auto& sphereTransform = sphere.getComponent<Transformation>();
			sphereTransform.setLocalPosition({ 0, 12, 2 });
			//boxTransform.rotate({ 0, 1, 0 }, 90);
			//boxTransform.rotate({ 0, 0, 1 }, 180);

			auto& rb = sphere.addComponent<RigidBodyComponent>();
			rb.mass = 1;
			rb.type = RigidbodyType::Dynamic;

			auto& collisionBox = sphere.addComponent<CollisionSphereComponent>();
			collisionBox.radius = 1.f;
		}

		sphere.setParent(box);


		{
			auto ground = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			auto& groundTransfrom = ground.getComponent<Transformation>();
			groundTransfrom.setLocalScale({ 10, .5f, 10 });
			//groundTransfrom.rotate({ 0, 0, 1 }, 20);
			//groundTransfrom.rotate({ 0, 0, 1 }, 90);

			auto& rb = ground.addComponent<RigidBodyComponent>();
			rb.type = RigidbodyType::Static;

			auto& collisionBox = ground.addComponent<CollisionBoxComponent>();
			collisionBox.halfExtent = .5f;
		}
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
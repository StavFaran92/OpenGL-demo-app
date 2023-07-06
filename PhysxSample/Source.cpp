#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:

	void start() override
	{
		Skybox::CreateSkybox();

		auto camera = std::dynamic_pointer_cast<EditorCamera>(Engine::get()->getContext()->getActiveScene()->getRenderer()->getCamera());
		camera->lookAt(0, 0, 0);
		camera->setPosition(13, 225, 35);

		{
			auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());

			auto& boxTransform = box.getComponent<Transformation>();
			boxTransform.setPosition({ 0, 10, .5f });
			boxTransform.rotate({ 0, 1, 0 }, 45);
			//boxTransform.rotate({ 0, 0, 1 }, 180);

			auto& rb = box.addComponent<RigidBodyComponent>();
			rb.mass = 1;
			rb.type = RigidbodyType::Dynamic;

			auto& collisionBox = box.addComponent<CollisionBoxComponent>();
			collisionBox.halfExtent = .5f;
		}

		{
			auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());

			auto& boxTransform = box.getComponent<Transformation>();
			boxTransform.setPosition({ 0, 12, 0 });
			//boxTransform.rotate({ 0, 1, 0 }, 90);
			//boxTransform.rotate({ 0, 0, 1 }, 180);

			auto& rb = box.addComponent<RigidBodyComponent>();
			rb.mass = 1;
			rb.type = RigidbodyType::Dynamic;

			auto& collisionBox = box.addComponent<CollisionBoxComponent>();
			collisionBox.halfExtent = .5f;
		}

		{
			auto sphere = ShapeFactory::createSphere(Engine::get()->getContext()->getActiveScene().get(), 1, 36, 36);

			auto& sphereTransform = sphere.getComponent<Transformation>();
			sphereTransform.setPosition({ 0, 15, 0 });
			//boxTransform.rotate({ 0, 1, 0 }, 90);
			//boxTransform.rotate({ 0, 0, 1 }, 180);

			auto& rb = sphere.addComponent<RigidBodyComponent>();
			rb.mass = 1;
			rb.type = RigidbodyType::Dynamic;

			auto& collisionBox = sphere.addComponent<CollisionSphereComponent>();
			collisionBox.radius = 1.f;
		}


		{
			auto ground = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			auto& groundTransfrom = ground.getComponent<Transformation>();
			groundTransfrom.setScale({ 10, .5f, 10 });
			//groundTransfrom.rotate({ 0, 1, 0 }, 90);
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
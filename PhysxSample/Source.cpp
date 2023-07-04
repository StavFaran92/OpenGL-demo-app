#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:

	void start() override
	{
		Skybox::CreateSkybox();

		{
			auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());

			auto& boxTransform = box.getComponent<Transformation>();
			boxTransform.setPosition({ 0, 10, 0 });

			auto& rb = box.addComponent<RigidBodyComponent>();
			rb.mass = 1;
			rb.type = RigidBodyComponent::RigidbodyType::Dynamic;

			auto& collisionBox = box.addComponent<CollisionBoxComponent>();
			collisionBox.halfExtent = .5f;
		}

		{
			//auto ground = ShapeFactory::createQuad(Engine::get()->getContext()->getActiveScene().get());
			//auto& groundTransfrom = ground.getComponent<Transformation>();
			//groundTransfrom.setScale({ 10, 1, 10 });
			//groundTransfrom.rotate({ 0, 1, 0 }, 90);
			//groundTransfrom.rotate({ 0, 0, 1 }, 90);

			//auto& rb = ground.addComponent<RigidBodyComponent>();
			//rb.type = RigidBodyComponent::RigidbodyType::Static;

			//auto& collisionBox = ground.addComponent<CollisionBoxComponent>();
			//collisionBox.halfExtent = 5.f;
		}
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
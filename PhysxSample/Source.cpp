#include "EntryPoint.h"
#include "sge.h"

void createSphere()
{
	auto sphere = ShapeFactory::createSphere(Engine::get()->getContext()->getActiveScene().get());
	{
		auto random = Engine::get()->getRandomSystem();
		auto x = random->rand() * 10 - 5;
		auto z = random->rand() * 10 - 5;

		auto& sphereTransform = sphere.getComponent<Transformation>();
		sphereTransform.setLocalPosition({ x, 10, z });


		auto& mat = sphere.addComponent<DefaultMaterial>(32.f);
		auto tex = Texture::loadTextureFromFile("Resources/Content/Textures/checkers.jpg");
		tex->setType(Texture::Type::Diffuse);
		mat.addTextureHandler(tex);

		auto& rb = sphere.addComponent<RigidBodyComponent>(RigidbodyType::Dynamic, 1.f);
		auto& collisionBox = sphere.addComponent<CollisionSphereComponent>(1.f);
	}
}

void createBox()
{
	auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
	{
		auto random = Engine::get()->getRandomSystem();
		auto x = random->rand() * 10 - 5;
		auto z = random->rand() * 10 - 5;

		auto& boxTransform = box.getComponent<Transformation>();
		boxTransform.setLocalPosition({ x, 10, z });
		boxTransform.rotate({ 0, 1, 0 }, 45);

		auto& mat = box.addComponent<DefaultMaterial>(32.f);
		auto tex = Texture::loadTextureFromFile("Resources/Content/Textures/checkers.jpg");
		tex->setType(Texture::Type::Diffuse);
		mat.addTextureHandler(tex);

		auto& rb = box.addComponent<RigidBodyComponent>(RigidbodyType::Dynamic, 1.f);
		auto& collisionBox = box.addComponent<CollisionBoxComponent>(.5f);
	}
}

class GUI_Helper : public GuiMenu
{
	// Inherited via GuiMenu
	virtual void display() override
	{
		ImGui::SetNextWindowPos({ 10, 10 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ 300, 250 }, ImGuiCond_Once);
		ImGui::Begin("Physx sample", 0, ImGuiWindowFlags_NoResize);

		ImGui::PushItemWidth(100);

		if (ImGui::Button("Create Box"))
		{
			createBox();
		}
		if (ImGui::Button("Create Sphere"))
		{
			createSphere();
		}

		//static float amplitude = 0.5f;

		//if (ImGui::InputFloat("amplitude", &amplitude))
		//	m_shader->setValue("amplitude", amplitude);

		//static auto waveDir = glm::vec2(1, 0);

		//if (ImGui::InputFloat2("wave Direction (x,y)", glm::value_ptr(waveDir)))
		//	m_shader->setValue("waveDirection", waveDir);

		//static float waveLength = 2.0f;

		//if (ImGui::InputFloat("wave Length", &waveLength))
		//	m_shader->setValue("waveLength", waveLength);

		//static float waveSpeed = 5.0f;

		//if (ImGui::InputFloat("wave Speed", &waveSpeed))
		//	m_shader->setValue("waveSpeed", waveSpeed);

		//static float steepness = .5f;

		//if (ImGui::SliderFloat("Steepness", &steepness, 0, 1))
		//	m_shader->setValue("steepness", steepness);

		ImGui::PopItemWidth();


		ImGui::End();
	}
};

class Sandbox : public Application
{
public:

	void start() override
	{
		Skybox::CreateSkybox();

		auto camera = Engine::get()->getContext()->getActiveScene()->getActiveCamera();
		camera->lookAt(0, 5, 0);
		camera->setPosition(25, 225, 35);

		auto gui = new GUI_Helper();
		Engine::get()->getImguiHandler()->addGUI(gui);

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
		//		//rb.type = RigidbodyType::Kinematic;

		//		//auto& rb = child.second.addComponent<RigidBodyComponent>();
		//		//rb.mass = 1;
		//		//rb.type = RigidbodyType::Dynamic;
		//	}
		//}


		//auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		//{

		//	auto& boxTransform = box.getComponent<Transformation>();
		//	boxTransform.setLocalPosition({ 0, 10, .5f });
		//	boxTransform.rotate({ 0, 1, 0 }, 45);
		//	//boxTransform.rotate({ 0, 0, 1 }, 180);

		//	auto& rb = box.addComponent<RigidBodyComponent>(RigidbodyType::Dynamic, 1.f);
		//	auto& collisionBox = box.addComponent<CollisionBoxComponent>(.5f);
		//}

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

		//auto sphere = ShapeFactory::createSphere(Engine::get()->getContext()->getActiveScene().get());
		//{
		//	
		//	auto& sphereTransform = sphere.getComponent<Transformation>();
		//	sphereTransform.setLocalPosition({ 0, 12, 2 });
		//	//boxTransform.rotate({ 0, 1, 0 }, 90);
		//	//boxTransform.rotate({ 0, 0, 1 }, 180);

		//	//auto& rb = sphere.addComponent<RigidBodyComponent>();
		//	//rb.mass = 1;
		//	//rb.type = RigidbodyType::Dynamic;


		//	auto& collisionBox = sphere.addComponent<CollisionSphereComponent>(1.f);
		//}

		//sphere.setParent(box);


		{
			auto ground = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			auto& groundTransfrom = ground.getComponent<Transformation>();
			groundTransfrom.setLocalScale({ 50, .5f, 50 });
			//groundTransfrom.rotate({ 0, 0, 1 }, 20);
			//groundTransfrom.rotate({ 0, 0, 1 }, 90);

			auto& mat = ground.addComponent<DefaultMaterial>(32.f);
			auto tex = Texture::loadTextureFromFile("Resources/Content/Textures/floor.jpg");
			tex->setType(Texture::Type::Diffuse);
			mat.addTextureHandler(tex);
			auto& rb = ground.addComponent<RigidBodyComponent>(RigidbodyType::Static, 1.f);


			auto& collisionBox = ground.addComponent<CollisionBoxComponent>(.5f);
		}
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
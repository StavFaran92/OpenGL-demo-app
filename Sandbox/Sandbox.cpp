#include "EntryPoint.h"
#include "sge.h"

#include "FlyCamera.h"
#include "PlayerController.h"
#include "EnemyController.h"

class CameraScript : public ScriptableEntity
{
	virtual void onCreate() override
	{
		Engine::get()->getWindow()->lockMouse();

		m_flyCamera = std::make_shared<FlyCamera>(entity, 0, 0, 3, 1);
	}

	virtual void onUpdate(float deltaTime) {
		m_flyCamera->onUpdate(deltaTime);
	};

	std::shared_ptr<FlyCamera> m_flyCamera;
};

class Sandbox : public Application
{
public:

	void start() override
	{
		Engine::get()->getInput()->getKeyboard()->onKeyPressed(SDL_SCANCODE_ESCAPE, [](SDL_Event e) {Engine::get()->stop(); });

		//Engine::get()->getSubSystem<Assets>()->importTexture("");

		SDL_ShowCursor(SDL_DISABLE);

		auto& camera = Engine::get()->getContext()->getActiveScene()->getActiveCamera();
		camera.addComponent<NativeScriptComponent>().bind<CameraScript>();
		auto& camTransform = camera.getComponent<Transformation>();
		//auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());

		Entity player = Engine::get()->getContext()->getActiveScene()->getEntityByName("Player");
		player.addComponent<NativeScriptComponent>().bind<PlayerController>(camera, 3);
		auto& playerTransform = player.getComponent<Transformation>();
		playerTransform.setWorldPosition({ 0, 10, 0 });

		camTransform.setLocalPosition(playerTransform.getLocalPosition());
		camTransform.translate({0,2,0});

		camera.setParent(player);

		Entity enemy_1 = Engine::get()->getContext()->getActiveScene()->getEntityByName("Enemy_1");
		enemy_1.addComponent<NativeScriptComponent>().bind<EnemyController>(3);

		Entity cursor = Engine::get()->getContext()->getActiveScene()->getEntityByName("Cursor");
		auto& cursorImage = cursor.getComponent<ImageComponent>();
		cursorImage.position.x = (Engine::get()->getWindow()->getWidth() - cursorImage.size.x ) / 2.f;
		cursorImage.position.y = (Engine::get()->getWindow()->getHeight() - cursorImage.size.y ) / 2.f;

		//camTransform.setLocalPosition(playerTransform.getLocalPosition());
		//camTransform.translate({0,2,0});

		//camera.setParent(player);

		//auto box1 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
		//auto& nsc = box1.addComponent<NativeScriptComponent>();
		//nsc.bind<CustomBoxBehaviour>();
		//box1->getComponent<Transformation>().translate({1, 0,0});
		//auto box2 = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());


		//auto importer = getContext()->getModelImporter();
		//auto guitar_deferred = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());
		//auto guitar_forward = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());
		//guitar_forward.getComponent<Transformation>().setLocalPosition({ 0,0,3 });
		//auto& renderable = guitar_forward.getComponent<RenderableComponent>();
		//renderable.renderTechnique = RenderableComponent::Forward;

		//auto tHandler = Texture::loadTextureFromFile("C:/Users/Stav/Downloads/gear_store_8k.hdr", Texture::Type::None);
		//Skybox::CreateSkybox(Skybox::TexType::EQUIRECTANGULAR, tHandler);


		//auto importer = getContext()->getModelImporter();

		//auto guitar = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());;

		//auto gun = importer->loadModelFromFile("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX", getContext()->getActiveScene().get());

		//auto& trans = gun.getComponent<Transformation>();
		//trans.scale({ .1f, .1f, .1f });
		//trans.rotate({ 1,0,0 }, -90);
		//trans.translate({0, 0,-5});
		//trans.rotate({ 0,1,0 }, 180);
	
		//auto box = ShapeFactory::createSphere(getContext()->getActiveScene().get());
		//auto& mat = gun.addComponent<Material>();
		//auto albedoMap = Engine::get()->getSubSystem<Assets>()->importTexture2D("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga", false);
		//mat.setTexture(Texture::Type::Albedo, Resource<Texture>(albedoMap));
		//auto roughnessMap = Engine::get()->getSubSystem<Assets>()->importTexture2D("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga", false);
		//mat.setTexture(Texture::Type::Roughness, Resource<Texture>(roughnessMap));
		//auto normalMap = Engine::get()->getSubSystem<Assets>()->importTexture2D("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga", false);
		//mat.setTexture(Texture::Type::Normal, Resource<Texture>(normalMap));
		//auto metallicMap = Engine::get()->getSubSystem<Assets>()->importTexture2D("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga", false);
		//mat.setTexture(Texture::Type::Metallic, Resource<Texture>(metallicMap));
		//auto aoMap = Engine::get()->getSubSystem<Assets>()->importTexture2D("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Raw/Cerberus_AO.tga", false);
		//mat.setTexture(Texture::Type::AmbientOcclusion, Resource<Texture>(aoMap));
		//box.getComponent<Transformation>().setLocalPosition({ i * 3,0,j * 3 });

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
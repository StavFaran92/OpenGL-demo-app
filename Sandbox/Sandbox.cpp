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


		//auto importer = getContext()->getModelImporter();
		//auto guitar_deferred = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());
		//auto guitar_forward = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());
		//guitar_forward.getComponent<Transformation>().setLocalPosition({ 0,0,3 });
		//auto& renderable = guitar_forward.getComponent<RenderableComponent>();
		//renderable.renderTechnique = RenderableComponent::Forward;

		Skybox::CreateSkybox({ SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/right.jpg",
		SGE_ROOT_DIR +"Resources/Engine/Textures/Skybox/left.jpg",
		SGE_ROOT_DIR +"Resources/Engine/Textures/Skybox/top.jpg",
		SGE_ROOT_DIR +"Resources/Engine/Textures/Skybox/bottom.jpg",
		SGE_ROOT_DIR +"Resources/Engine/Textures/Skybox/front.jpg",
		SGE_ROOT_DIR +"Resources/Engine/Textures/Skybox/back.jpg" });

		//auto tHandler = Texture::loadTextureFromFile("C:/Users/Stav/Downloads/gear_store_8k.hdr", Texture::Type::None);
		//Skybox::CreateSkybox(Skybox::TexType::EQUIRECTANGULAR, tHandler);


		auto importer = getContext()->getModelImporter();

		//auto guitar = importer->loadModelFromFile("C:/Users/Stav/Downloads/backpack/backpack.obj", getContext()->getActiveScene().get());;

		auto gun = importer->loadModelFromFile("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX", getContext()->getActiveScene().get());

		auto& trans = gun.getComponent<Transformation>();
		trans.scale({ .1f, .1f, .1f });
		trans.rotate({ 1,0,0 }, -90);
		trans.translate({0, 0,-5});
		//trans.rotate({ 0,1,0 }, 180);
	
		//auto box = ShapeFactory::createSphere(getContext()->getActiveScene().get());
		auto& mat = gun.addComponent<Material>();
		auto albedoMap = Texture::loadTextureFromFile("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga", false);
		mat.setTexture(Texture::Type::Albedo, Resource<Texture>(albedoMap));
		auto roughnessMap = Texture::loadTextureFromFile("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga", false);
		mat.setTexture(Texture::Type::Roughness, Resource<Texture>(roughnessMap));
		auto normalMap = Texture::loadTextureFromFile("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga", false);
		mat.setTexture(Texture::Type::Normal, Resource<Texture>(normalMap));
		auto metallicMap = Texture::loadTextureFromFile("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga", false);
		mat.setTexture(Texture::Type::Metallic, Resource<Texture>(metallicMap));
		auto aoMap = Texture::loadTextureFromFile("C:/Users/Stav/Downloads/Cerberus_by_Andrew_Maximov/Textures/Raw/Cerberus_AO.tga", false);
		mat.setTexture(Texture::Type::AmbientOcclusion, Resource<Texture>(aoMap));
		//box.getComponent<Transformation>().setLocalPosition({ i * 3,0,j * 3 });

		/*for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				auto box = ShapeFactory::createSphere(getContext()->getActiveScene().get());
				auto& mat = box.getComponent<Material>();
				auto albedoMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_albedo.png", Texture::Type::Albedo);
				mat.setTexture(Texture::Type::Albedo, Resource<Texture>(albedoMap));
				auto roughnessMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_roughness.png", Texture::Type::Roughness);
				mat.setTexture(Texture::Type::Roughness, Resource<Texture>(roughnessMap));
				auto normalMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_normal-ogl.png", Texture::Type::Normal);
				mat.setTexture(Texture::Type::Normal, Resource<Texture>(normalMap));
				auto metallicMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_metallic.png", Texture::Type::Metallic);
				mat.setTexture(Texture::Type::Metallic, Resource<Texture>(metallicMap));
				auto aoMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_ao.png", Texture::Type::AmbientOcclusion);
				mat.setTexture(Texture::Type::AmbientOcclusion, Resource<Texture>(aoMap));
				box.getComponent<Transformation>().setLocalPosition({ i * 3,0,j*3 });
			}
		}*/
	



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
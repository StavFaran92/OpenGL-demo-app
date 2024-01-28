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



		//auto importer = getContext()->getModelImporter();

		//auto helmet = importer->loadModelFromFile("./Resources/Content/Model/source/HelmetPresentationLightMap.fbx.fbx", getContext()->getActiveScene().get());

		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				auto box = ShapeFactory::createBox(getContext()->getActiveScene().get());
				auto& mat = box.getComponent<Material>();
				auto albedoMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_albedo.png", Texture::Type::Albedo);
				mat.setTexture(Texture::Type::Albedo, std::shared_ptr<TextureHandler>(albedoMap));
				auto roughnessMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_roughness.png", Texture::Type::Roughness);
				mat.setTexture(Texture::Type::Roughness, std::shared_ptr<TextureHandler>(roughnessMap));
				auto normalMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_normal-ogl.png", Texture::Type::Normal);
				mat.setTexture(Texture::Type::Normal, std::shared_ptr<TextureHandler>(normalMap));
				auto metallicMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_metallic.png", Texture::Type::Metallic);
				mat.setTexture(Texture::Type::Metallic, std::shared_ptr<TextureHandler>(metallicMap));
				auto aoMap = Texture::loadTextureFromFile("./Resources/Content/Model/glossy-marble-tile-bl/glossy-marble-tile_ao.png", Texture::Type::AmbientOcclusion);
				mat.setTexture(Texture::Type::AmbientOcclusion, std::shared_ptr<TextureHandler>(aoMap));
				box.getComponent<Transformation>().setLocalPosition({ i * 2,0,j*2 });
			}
		}
	



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
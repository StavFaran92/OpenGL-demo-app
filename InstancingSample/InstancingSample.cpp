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
		//Skybox::CreateSkybox({ SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/right.jpg",
		//SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/left.jpg",
		//SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/top.jpg",
		//SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/bottom.jpg",
		//SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/front.jpg",
		//SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/back.jpg" });

		

		auto importer = getContext()->getModelImporter();
		auto rock = importer->loadModelFromFile("C:/Users/Stav/Downloads/rock1-ue/rock2.obj", getContext()->getActiveScene().get());

		auto rockMat = std::make_shared<Material>();
		
		auto albedoMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-albedo.png", false);
		rockMat->setTexture(Texture::Type::Albedo, Resource<Texture>(albedoMap));
		auto roughnessMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-roughness.png", false);
		rockMat->setTexture(Texture::Type::Roughness, Resource<Texture>(roughnessMap));
		auto normalMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-normal_dx.png", false);
		rockMat->setTexture(Texture::Type::Normal, Resource<Texture>(normalMap));
		auto metallicMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-metallic.png", false);
		rockMat->setTexture(Texture::Type::Metallic, Resource<Texture>(metallicMap));
		auto aoMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-ao.png", false);
		rockMat->setTexture(Texture::Type::AmbientOcclusion, Resource<Texture>(aoMap));

		rock.getComponent<MaterialComponent>().addMaterial(rockMat);

		std::vector<Transformation> transformations;

		const int rocksCount = 50;
		
		auto random = Engine::get()->getRandomSystem();
		

		for (int i = 0; i < rocksCount; i++)
		{
			float angle = (float)i * 2 * Constants::PI / rocksCount;
			Transformation trans(rock);
			trans.setLocalPosition({ cos(angle) * 200, 0, sin(angle) * 200 });
			trans.setLocalScale({ .05f + random->rand() * .05f, .1f + random->rand() * .05f, .1f + random->rand() * .05f });
			transformations.push_back(trans);
		}

		auto& meshComponent = rock.getComponent<MeshComponent>();

		rock.addComponent<InstanceBatch>(transformations, meshComponent.mesh);

		auto sphere = ShapeFactory::createSphere(&getContext()->getActiveScene()->getRegistry());
		auto& trans = sphere.getComponent<Transformation>();
		trans.setLocalScale({ 10, 10, 10 });

		auto sphereMat = std::make_shared<Material>();
		//sphereMat->setTexture(Texture::Type::Albedo, Resource<Texture>(albedoMap));
		//sphereMat->setTexture(Texture::Type::Metallic, Resource<Texture>(metallicMap));
		sphere.getComponent<MaterialComponent>().addMaterial(sphereMat);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
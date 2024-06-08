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

		auto& trans = rock.getComponent<Transformation>();
		trans.setLocalScale({.1f, .1f, .1f});

		auto mat = std::make_shared<Material>();
		
		auto albedoMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-albedo.png", false);
		mat->setTexture(Texture::Type::Albedo, Resource<Texture>(albedoMap));
		auto roughnessMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-roughness.png", false);
		mat->setTexture(Texture::Type::Roughness, Resource<Texture>(roughnessMap));
		auto normalMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-normal_dx.png", false);
		mat->setTexture(Texture::Type::Normal, Resource<Texture>(normalMap));
		auto metallicMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-metallic.png", false);
		mat->setTexture(Texture::Type::Metallic, Resource<Texture>(metallicMap));
		auto aoMap = Texture::create2DTextureFromFile("C:/Users/Stav/Downloads/rock1-ue/rock1-ao.png", false);
		mat->setTexture(Texture::Type::AmbientOcclusion, Resource<Texture>(aoMap));

		rock.addComponent<MaterialComponent>().addMaterial(mat);

		//std::vector<Transformation> transformations;

		//const int gridLength = 100;

		//for (int i = 0; i < gridLength; i++)
		//{
		//	for (int j = 0; j < gridLength; j++)
		//	{
		//		Transformation trans(box, {(i - gridLength / 2) * 2, 0, (j - gridLength / 2) * 2});
		//		transformations.push_back(trans);
		//	}
		//}

		//auto& meshComponent = box.getComponent<MeshComponent>();

		//box.addComponent<InstanceBatch>(transformations, meshComponent.mesh);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
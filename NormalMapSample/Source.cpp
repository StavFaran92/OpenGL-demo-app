#include "EntryPoint.h"
#include "sge.h"


class CustomBoxBehaviour : public ScriptableEntity
{
	virtual void onUpdate(float dt) override
	{
		static float theta = 0;
		theta += .01f;
		auto& trans = entity.getComponent<Transformation>();
		trans.setLocalPosition({ cos(theta), 1, sin(theta) });
	}
};

class Sandbox : public Application
{
public:

	void start() override
	{
		//auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());


		{
			auto ground = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			auto& groundTransfrom = ground.getComponent<Transformation>();
			groundTransfrom.setLocalScale({ 10, .1f, 10 });
			auto& mat = ground.addComponent<Material>();
			auto texDiff = Texture::loadTextureFromFile("Resources/Content/Textures/brickwall.jpg", Texture::Type::Diffuse);
			mat.setTexture(Texture::Type::Diffuse, std::shared_ptr<TextureHandler>(texDiff));
			auto texNorm = Texture::loadTextureFromFile("Resources/Content/Textures/brickwall_normal.jpg", Texture::Type::Normal);
			mat.setTexture(Texture::Type::Normal, std::shared_ptr<TextureHandler>(texNorm));
		}

		auto pLight = getContext()->getActiveScene()->createEntity();
		pLight.addComponent<PointLight>(glm::vec3{ 1,1,1 }, 1.f, 1.f, Attenuation());
		pLight.addComponent<Transformation>(pLight, glm::vec3{ 0,2,0 });
		pLight.addComponent<NativeScriptComponent>().bind<CustomBoxBehaviour>();


		//postProcess(PostProcess::grayscale());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
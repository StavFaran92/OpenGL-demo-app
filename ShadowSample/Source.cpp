#include "EntryPoint.h"
#include "sge.h"

class CustomBoxBehaviour : public ScriptableEntity
{
	virtual void onUpdate(float dt) override
	{
		static float theta = 0;
		theta += .01f;
		auto& dirLight = entity.getComponent<DirectionalLight>();
		dirLight.setDirection({ cos(theta), -1, sin(theta) });
	}
};

class Sandbox : public Application
{
public:

	void start() override
	{
		//auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());

		{
			auto box = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			box.getComponent<Transformation>().translate({ 0, 2, 0 });
			auto& mat = box.addComponent<Material>();
			auto tex = Texture::loadTextureFromFile("Resources/Content/Textures/checkers.jpg");
			mat.setTexture(Texture::Type::Diffuse, std::shared_ptr<TextureHandler>(tex));
			//auto& nsc = box.addComponent<NativeScriptComponent>();
			//nsc.bind<CustomBoxBehaviour>();
		}


		{
			auto ground = ShapeFactory::createBox(Engine::get()->getContext()->getActiveScene().get());
			auto& groundTransfrom = ground.getComponent<Transformation>();
			groundTransfrom.setLocalScale({ 50, .5f, 50 });
			auto& mat = ground.addComponent<Material>();
			auto tex = Texture::loadTextureFromFile("Resources/Content/Textures/floor.jpg");
			mat.setTexture(Texture::Type::Diffuse, std::shared_ptr<TextureHandler>(tex));
		}

		auto entt = Engine::get()->getContext()->getActiveScene()->getRegistry().view<DirectionalLight>().front();
		Entity e{ entt, Engine::get()->getContext()->getActiveScene().get() };
		e.addComponent<NativeScriptComponent>().bind<CustomBoxBehaviour>();

		

		//postProcess(PostProcess::grayscale());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
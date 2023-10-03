#include "EntryPoint.h"
#include "sge.h"


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
			auto tex = Texture::loadTextureFromFile("Resources/Content/Textures/floor.jpg", Texture::Type::Diffuse);
			mat.setTexture(Texture::Type::Diffuse, std::shared_ptr<TextureHandler>(tex));
		}


		//postProcess(PostProcess::grayscale());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
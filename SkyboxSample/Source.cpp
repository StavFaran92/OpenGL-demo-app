#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:

	void start() override
	{
		auto skybox = Skybox::CreateSkybox(Engine::get()->getContext()->getActiveScene().get());




		//postProcess(PostProcess::grayscale());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
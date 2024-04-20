#include "EntryPoint.h"
#include "sge.h"

class Sandbox : public Application
{
public:

	void start() override
	{
		Skybox::CreateSkybox({ SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/right.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/left.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/top.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/bottom.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/front.jpg",
		SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/back.jpg" });




		//postProcess(PostProcess::grayscale());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
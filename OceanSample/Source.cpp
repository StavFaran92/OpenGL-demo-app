#include "EntryPoint.h"
#include "sge.h"

#include "GUIHandler.h"


class Sandbox : public Application
{
public:
	void start() override
	{
		auto importer = getContext()->getModelImporter();
		auto quad = importer->loadModelFromFile("C:/Users/Stav/Documents/blender/plane_v2.fbx", getContext()->getActiveScene().get());

		auto& planeTransform = quad.getComponent<Transformation>();
		planeTransform.rotate({ 1,0,0 }, 90);
		planeTransform.rotate({ 0,1,0 }, 90);

		//planeModel.object()->rotate({1,0,0}, 90);
		//planeModel.object()->rotate({0,1,0}, 90);
		//plane.object()->scale(10, 1, 10);

		Shader* shader = ShaderBuilder::create("Resources/Content/Shaders/OceanShader.glsl").build();

		quad.addComponent<ShaderComponent>(shader);

		auto gui = new GUIHandler(shader);
		Engine::get()->getImguiHandler()->addGUI(gui);

	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
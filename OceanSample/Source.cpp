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

		auto& planeTransform = planeModel.getComponent<Transformation>();
		planeTransform.rotate({ 1,0,0 }, 90);
		planeTransform.rotate({ 0,1,0 }, 90);

		//planeModel.object()->rotate({1,0,0}, 90);
		//planeModel.object()->rotate({0,1,0}, 90);
		//plane.object()->scale(10, 1, 10);

		StandardShader* shader = Shader::create<StandardShader>("Resources/Content/Shaders/OceanVertexShader.vert", "Resources/Content/Shaders/OceanFragmentShader.frag");
		shader->setValue("amplitude", .5f);
		shader->setValue("waveDirection", glm::vec2(1,0));
		shader->setValue("waveLength", 2.f);
		shader->setValue("waveSpeed", 5.0f);
		shader->setValue("steepness", .5f);
		auto& shaderRef = planeModel.addComponent<StandardShader>(shader);

		auto dLight = getContext()->getActiveScene()->createEntity();
		dLight.addComponent<PointLight>(glm::vec3{ 1,1,1 }, 1.f, 1.f, Attenuation());
		dLight.addComponent<Transformation>(dLight, glm::vec3{ 0,0,2 });

		auto gui = new GUIHandler(&shaderRef);
		Engine::get()->getImguiHandler()->addGUI(gui);

	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
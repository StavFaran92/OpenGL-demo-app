#include "EntryPoint.h"
#include "sge.h"

#include "GUIHandler.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;


	ObjectHandler<Model> plane;
	ObjectHandler<Model> light;
	std::shared_ptr<GUIHandler> gui;


	void start() override
	{
		auto importer = getContext()->getModelImporter();
		plane = importer->loadModelFromFile("C:/Users/Stav/Documents/blender/plane_v2.fbx");

		plane.object()->rotate({1,0,0}, 90);
		plane.object()->rotate({0,1,0}, 90);
		//plane.object()->scale(10, 1, 10);

		StandardShader* shader = Shader::create<StandardShader>("Resources/Content/Shaders/OceanVertexShader.vert", "Resources/Content/Shaders/OceanFragmentShader.frag");
		shader->setValue("amplitude", .5f);
		shader->setValue("waveDirection", glm::vec2(1,0));
		shader->setValue("waveLength", 2.f);
		shader->setValue("waveSpeed", 5.0f);
		shader->setValue("steepness", .5f);
		plane.object()->attachShader(shader);

		auto dLight = new PointLight(glm::vec3{ 1,1,1 }, glm::vec3{ 0, 0, 2 }, 1, 1, Attenuation());
		getContext()->getActiveScene()->addPointLight(dLight);

		auto scene = getContext()->getActiveScene();
		Entity e ( scene.get() );
		e.addComponent<Model>(plane.object());

		//getContext()->getActiveScene()->addModel(plane.object());

		gui = std::make_shared<GUIHandler>(shader);

	}

	void update(float deltaTime) override
	{
		//update(plane);
	}

	void draw() override
	{
		//getContext()->getActiveScene()->draw(plane);

		Engine::get()->getImguiHandler()->draw(gui.get());
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}
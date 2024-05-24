#pragma once

#include <map>
#include <memory>
#include "Core.h"
#include "Resource.h"

class Model;
class Shader;
class IRenderer;
class PointLight;
class DirectionalLight;
class Skybox;
class Object3D;
class Scene;
class Engine;
class PhongShader;
class PickingShader;
class Window;
class ImguiHandler;
class Input;
class EventSystem;
class ModelImporter;
class TextureHandler;
class Material;
class ProjectAssetRegistry;



class EngineAPI Context
{
public:
	Context(const std::shared_ptr<ProjectAssetRegistry>& par);

	//bool addObject(std::shared_ptr<Object3D> object);
	//bool removeObject(std::shared_ptr <Object3D> object);

	bool addScene(std::shared_ptr<Scene> scene);
	bool removeScene(std::shared_ptr<Scene> scene);

	bool AddShader(std::shared_ptr<Shader> shader);
	bool RemoveShader(std::shared_ptr<Shader> shader);

	Window* getWindow() const;
	IRenderer* getRenderer() const;
	ImguiHandler* getImguiHandler() const;
	Input* getInput() const;
	EventSystem* getEventSystem() const;
	ModelImporter* getModelImporter() const;
	//std::shared_ptr<Material> getDefaultMaterial() const;

	std::shared_ptr<Scene> getActiveScene() const;

	ProjectAssetRegistry* getProjectAssetRegistry() const;

	void populateScenesFromJSON(const std::string& json);
	

	void setActiveScene(uint32_t index);

	//Resource<Texture> getDummyTexture();

	void close();

private:
	friend class Engine;
	void update(float deltaTime);
	void draw(float deltaTime);
	void init();

	int m_activeScene = -1;
	uint32_t m_scenesCounter = 0;
	std::map<uint32_t, std::shared_ptr<Scene>> m_scenes;

	std::map<uint32_t, std::shared_ptr<Shader>> m_shaders;
	uint32_t m_shaderCounter = 0;

	std::shared_ptr<ProjectAssetRegistry> m_projectAssetRegistry;
};



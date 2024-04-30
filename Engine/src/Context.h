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



class EngineAPI Context
{
public:
	Context();

	//bool addObject(std::shared_ptr<Object3D> object);
	//bool removeObject(std::shared_ptr <Object3D> object);

	bool addScene(std::shared_ptr<Scene> scene);
	bool removeScene(std::shared_ptr<Scene> scene);

	bool AddShader(std::shared_ptr<Shader> shader);
	bool RemoveShader(std::shared_ptr<Shader> shader);

	Shader* GetReflectionShader() const;
	Shader* GetRefractiveShader() const;
	Shader* getStandardShader() const;
	PickingShader* getPickingShader() const;
	Shader* getNormalDisplayShader() const;

	Window* getWindow() const;
	IRenderer* getRenderer() const;
	ImguiHandler* getImguiHandler() const;
	Input* getInput() const;
	EventSystem* getEventSystem() const;
	ModelImporter* getModelImporter() const;

	std::shared_ptr<Scene> getActiveScene() const;
	

	void setActiveScene(uint32_t index);

	Resource<Texture> getDummyTexture();

private:
	friend class Engine;
	void update(float deltaTime);
	void draw(float deltaTime);

	uint32_t m_activeScene = -1;
	uint32_t m_scenesCounter = 0;
	std::map<uint32_t, std::shared_ptr<Scene>> m_scenes;

	std::map<uint32_t, std::shared_ptr<Shader>> m_shaders;
	uint32_t m_shaderCounter = 0;

	std::shared_ptr<Shader> m_reflectionShader = nullptr;
	std::shared_ptr<Shader> m_refractiveShader = nullptr;
	std::shared_ptr<Shader> m_phongShader = nullptr;
	std::shared_ptr<PickingShader> m_pickingShader = nullptr;
	std::shared_ptr<Shader> m_normalDisplayShader = nullptr;
	Resource<Texture> m_dummyTexture;
};



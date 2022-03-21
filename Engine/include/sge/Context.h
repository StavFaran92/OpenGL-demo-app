#pragma once

#include <map>
#include <memory>
#include "Core.h"

class Model;
class Shader;
class Renderer;
class PointLight;
class DirectionalLight;
class Skybox;
class Object3D;
class Scene;
class Engine;

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

	std::shared_ptr<Shader> GetReflectionShader();
	std::shared_ptr<Shader> GetRefractiveShader();

	std::shared_ptr<Scene> getActiveScene() const
	{
		if (m_activeScene == -1)
			return nullptr;

		return m_scenes.at(m_activeScene);
	}

	void setActiveScene(uint32_t index);
	


private:
	friend class Engine;
	void update(float deltaTime);
	void draw();

	uint32_t m_activeScene = -1;
	uint32_t m_scenesCounter = 0;
	std::map<uint32_t, std::shared_ptr<Scene>> m_scenes;

	//std::map<uint32_t, std::shared_ptr<Object3D>> m_objects;
	//uint32_t m_objCounter = 0;

	std::map<uint32_t, std::shared_ptr<Shader>> m_shaders;
	uint32_t m_shaderCounter = 0;

	std::shared_ptr<Shader> m_reflectionShader = nullptr;
	std::shared_ptr<Shader> m_refractiveShader = nullptr;
};


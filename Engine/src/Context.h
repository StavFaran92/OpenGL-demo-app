#pragma once

#include <map>
#include <memory>

#include "Logger.h"

class Model;
class Shader;
class Renderer;
class PointLight;
class DirectionalLight;
class Skybox;
class Object3D;

class Context
{
public:
	Context();

	bool addObject(std::shared_ptr<Object3D> object);
	bool removeObject(std::shared_ptr <Object3D> object);

	//bool AddModel(std::shared_ptr<Model> model);
	//bool RemoveModel(std::shared_ptr<Model> model);

	bool AddShader(std::shared_ptr<Shader> shader);
	bool RemoveShader(std::shared_ptr<Shader> shader);

	//bool AddPointLight(std::shared_ptr<PointLight> light);
	//bool RemovePointLight(std::shared_ptr<PointLight> light);

	//bool AddDirectionalLight(std::shared_ptr<DirectionalLight> light);
	//bool RemoveDirectionalLight(std::shared_ptr<DirectionalLight> light);

	//bool AddSkybox(std::shared_ptr<Skybox> skybox);
	//bool RemoveSkybox(std::shared_ptr<Skybox> skybox);

	std::shared_ptr<Shader> GetReflectionShader();
	std::shared_ptr<Shader> GetRefractiveShader();
	std::shared_ptr<Model> GetSkyBox();

	std::shared_ptr<Renderer> GetRenderer() { return m_renderer; }

	//void Update(float deltaTime);
	//void Draw();
private:
	std::shared_ptr<Renderer> m_renderer = nullptr;
	std::shared_ptr<Renderer> m_skyboxRenderer = nullptr;

	std::map<uint32_t, std::shared_ptr<Object3D>> m_objects;
	uint32_t m_objCounter = 0;

	//std::map<uint32_t, std::shared_ptr<Model>> m_models;
	//uint32_t m_modelCounter = 0;

	std::map<uint32_t, std::shared_ptr<Shader>> m_shaders;
	uint32_t m_shaderCounter = 0;

	//std::map<uint32_t, std::shared_ptr<PointLight>> m_pointLights;
	//uint32_t m_pointLightCounter = 0;

	//std::map<uint32_t, std::shared_ptr<DirectionalLight>> m_directionalLights;
	//uint32_t m_directionalLightCounter = 0;

	//std::map<uint32_t, std::shared_ptr<Skybox>> m_skyboxes;
	//uint32_t m_skyboxCounter = 0;

	std::shared_ptr<Shader> m_reflectionShader = nullptr;
	std::shared_ptr<Shader> m_refractiveShader = nullptr;
};


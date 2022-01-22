#pragma once

#include <map>
#include <memory>

#include "Utils/Logger/Logger.h"

class Model;
class Shader;
class Renderer;
class PointLight;
class DirectionalLight;
class Skybox;

class Context
{
public:
	Context();

	bool AddModel(std::shared_ptr<Model> model);
	bool RemoveModel(const uint32_t uid);

	bool AddShader(std::shared_ptr<Shader> shader);
	bool RemoveShader(const uint32_t uid);

	bool AddPointLight(std::shared_ptr<PointLight> light);
	bool RemovePointLight(const uint32_t uid);

	bool AddDirectionalLight(std::shared_ptr<DirectionalLight> light);
	bool RemoveDirectionalLight(const uint32_t uid);

	bool AddSkybox(std::shared_ptr<Skybox> skybox);
	bool RemoveSkybox(const uint32_t uid);

	std::shared_ptr<Renderer> GetRenderer() { return m_renderer; }

	void Update(float deltaTime);
	void Draw();
private:
	std::shared_ptr<Renderer> m_renderer = nullptr;
	std::shared_ptr<Renderer> m_skyboxRenderer = nullptr;

	std::map<uint32_t, std::shared_ptr<Model>> m_models;
	uint32_t m_modelCounter = 0;

	std::map<uint32_t, std::shared_ptr<Shader>> m_shaders;
	uint32_t m_shaderCounter = 0;

	std::map<uint32_t, std::shared_ptr<PointLight>> m_pointLights;
	uint32_t m_pointLightCounter = 0;

	std::map<uint32_t, std::shared_ptr<DirectionalLight>> m_directionalLights;
	uint32_t m_directionalLightCounter = 0;

	std::shared_ptr<Skybox> m_skybox;
};


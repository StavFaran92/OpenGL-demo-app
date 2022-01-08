#pragma once

#include <map>
#include <memory>

#include "Utils/Logger/Logger.h"

class Model;
class Shader;
class Renderer;

class Context
{
public:
	Context() : m_modelCounter(0) , m_shaderCounter(0)
	{
		m_renderer = std::make_shared<Renderer>();
	}

	bool AddModel(std::shared_ptr<Model> model);
	bool RemoveModel(const uint32_t uid);

	bool AddShader(std::shared_ptr<Shader> shader);
	bool RemoveShader(const uint32_t uid);

	std::shared_ptr<Renderer> GetRenderer() { return m_renderer; }

	void Update(float deltaTime);
	void Draw();
private:
	std::map<uint32_t, std::shared_ptr<Model>> m_models;
	uint32_t m_modelCounter = 0;

	std::shared_ptr<Renderer> m_renderer = nullptr;

	std::map<uint32_t, std::shared_ptr<Shader>> m_shaders;
	uint32_t m_shaderCounter = 0;
};


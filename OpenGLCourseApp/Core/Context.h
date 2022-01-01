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
	{}

	bool AddModel(std::shared_ptr<Model> model);
	bool RemoveModel(const uint32_t uid);

	bool AddShader(std::shared_ptr<Shader> shader);
	bool RemoveShader(const uint32_t uid);

	void Update(float deltaTime);
	void Draw(std::shared_ptr<Renderer> renderer);
private:
	std::map<uint32_t, std::shared_ptr<Model>> m_models;
	uint32_t m_modelCounter = 0;

	std::map<uint32_t, std::shared_ptr<Shader>> m_shaders;
	uint32_t m_shaderCounter = 0;
};


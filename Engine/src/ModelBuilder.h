#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <memory>

#include "Core.h"

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "MeshBuilder.h"
#include "Texture.h"

class EngineAPI ModelBuilder
{
public:
	/** Constructor */
	ModelBuilder() = default;

	template<typename T, typename... _Types>
	void init(_Types&&... _Args);

	/** Destructor */
	~ModelBuilder() {};

	template<typename T, typename... _Types>
	static ModelBuilder builder(_Types&&... _Args);

	ModelBuilder& setShader(Shader& shader, bool copy = false);

	MeshBuilder& getMeshBuilder();
	
	Model* build();
private:
	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<MeshBuilder> m_meshBuilder = nullptr;
	std::shared_ptr<Model> m_model = nullptr;

	bool isBuilt = false;

};

template<typename T, typename... _Types>
void ModelBuilder::init(_Types&&... _Args)
{
	m_model = std::make_shared<T>(std::forward<_Types>(_Args)...);

	m_meshBuilder = std::shared_ptr<MeshBuilder>(m_model->createMeshBuilder());
	m_meshBuilder->setModelBuilder(this);
}


template<typename T, typename... _Types>
ModelBuilder ModelBuilder::builder(_Types&&... _Args)
{
	static_assert(std::is_base_of<Model, T>::value, "T must inherit from Model");

	auto builder = ModelBuilder();
	builder.init<T>(std::forward<_Types>(_Args)...);

	return builder;
}
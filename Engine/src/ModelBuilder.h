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
#include "ObjectFactory.h"
#include "ObjectHandler.h"

class StandardShader;

// 
// Create a default Box -> Build
// ```
//	Box* box = (Box*)ModelBuilder::builder<Box>().build();
// ```

// Create a default Box -> Go to it's mesh builder -> add a texture to the mesh -> return to the model builder -> Build
// ```
//	(Box*)ModelBuilder::builder<Box>()
//		.getMeshBuilder()
//		.addTexture(texture)
//		.getModelBuilder()
//		.build();
// ```

//// Create a default Model ->  Set it's vertices data -> Set it's shader -> Build
// ```
//	Mesh::VerticesLayout layout;
//	layout.numOfVertices = 3;
//	layout.attribs.push_back(LayoutAttributes::Positions);
//	
//	ModelBuilder::builder<Model>()
//		.getMeshBuilder()
//		.setRawVertices(vertices, layout)
//		.getModelBuilder()
//		.setShader(shader)
//		.build();
// ```

// Create a sphere-> Set it's position and Colors ->  Build
// ```
//	float positions[10] = { /*...*/ };
//	std::vector<glm::vec3> colors = { /*...*/ };
//	
//	sphere = (Sphere*)ModelBuilder::builder<Sphere>(2, 36, 18)
//		.getMeshBuilder()
//		.setPositions(positions, 10)
//		.setColors(colors)
//		.getModelBuilder()
//		.build();
// ```

class EngineAPI ModelBuilder
{
public:
	/** Constructor */
	ModelBuilder();

	/** Destructor */
	~ModelBuilder() = default;

	template<typename T, typename... _Types>
	static ModelBuilder& builder(_Types&&... _Args);

	ModelBuilder& setShader(StandardShader* shader, bool copy = false);

	ModelBuilder& addTextureHandler(TextureHandler* textureHandler, bool copy = false);
	ModelBuilder& addTextureHandlers(std::vector<TextureHandler*>& textureHandlers, bool copy = false);

	MeshBuilder& getMeshBuilder();
	
	ObjectHandler<Model> build();
private:
	template<typename T, typename... _Types>
	void init(_Types&&... _Args);
private:
	std::shared_ptr<StandardShader> m_shader = nullptr;
	std::shared_ptr<std::vector<TextureHandler*>> m_textureHandlers = nullptr;

	MeshBuilder* m_meshBuilder = nullptr;
	ObjectHandler<Model> m_modelHandler;

	bool isBuilt = false;

};

template<typename T, typename... _Types>
void ModelBuilder::init(_Types&&... _Args)
{
	m_modelHandler = ObjectFactory::create<T>(std::forward<_Types>(_Args)...);

	m_meshBuilder = m_modelHandler.object()->createMeshBuilder();
	m_meshBuilder->setModelBuilder(this);
}


template<typename T, typename... _Types>
ModelBuilder& ModelBuilder::builder(_Types&&... _Args)
{
	static_assert(std::is_base_of<Model, T>::value, "T must inherit from Model");

	auto builder = new ModelBuilder();
	builder->init<T>(std::forward<_Types>(_Args)...);

	return *builder;
}
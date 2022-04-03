#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <memory>

#include <any>
#include "Core.h"

#include "Mesh.h"

template<typename T>
class ModelBuilder;

class EngineAPI MeshBuilder
{
public:
	/** Constructor */
	MeshBuilder() = default;

	/** Destructor */
	~MeshBuilder() = default;

	bool isBuilt() const;

	MeshBuilder& setNumOfVertices(size_t size);

	MeshBuilder& setPositions(std::vector<glm::vec3>& positions, bool copy = false);
	MeshBuilder& setPositions(const float* positions, size_t size);

	MeshBuilder& setNormals(std::vector<glm::vec3>& normals, bool copy = false);
	MeshBuilder& setNormals(const float* normals, size_t size);

	MeshBuilder& setTexcoords(std::vector<glm::vec2>& texCoords, bool copy = false);
	MeshBuilder& setTexcoords(const float* texCoords, size_t size);

	MeshBuilder& setColors(std::vector<glm::vec3>& colors, bool copy = false);

	MeshBuilder& setColors(const float* colors, size_t size);

	MeshBuilder& setIndices(std::vector<unsigned int>& indices, bool copy = false);

	MeshBuilder& setRawVertices(const float* vertices, Mesh::VerticesLayout layout);

	MeshBuilder& setRawIndices(const unsigned int* indices, size_t size);

	Mesh* build();

	template<typename T>
	void setModelBuilder(ModelBuilder<T>* modelBuilder);

	template<typename T>
	ModelBuilder<T>& getModelBuilder();
	
private:
	size_t m_numOfVertices = 0;
	std::shared_ptr<std::vector<glm::vec3>> m_positions = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> m_normals = nullptr;
	std::shared_ptr<std::vector<glm::vec2>> m_texCoords = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> m_colors = nullptr;
	std::shared_ptr<std::vector<unsigned int>> m_indices = nullptr;

	std::any m_modelBuilder;

	bool m_isBuilt = false;

};

template<typename T>
void MeshBuilder::setModelBuilder(ModelBuilder<T>* modelBuilder)
{
	m_modelBuilder = modelBuilder;
}

template<typename T>
ModelBuilder<T>& MeshBuilder::getModelBuilder()
{
	return std::any_cast<ModelBuilder<T>&>(m_modelBuilder);
}

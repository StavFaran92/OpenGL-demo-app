#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <memory>

#include <any>
#include "Core.h"
#include "Configurations.h"

#include "Mesh.h"
#include "Model.h"
#include "VertexLayout.h"

class ModelBuilder;
class Texture;

class EngineAPI MeshBuilder
{
public:
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

	MeshBuilder& setRawVertices(const float* vertices, VertexLayout layout);

	MeshBuilder& setRawIndices(const unsigned int* indices, size_t size);

	MeshBuilder& addTexture(std::shared_ptr<Texture>& texture, bool copy = false);
	MeshBuilder& addTextures(std::vector<std::shared_ptr<Texture>>& textures, bool copy = false);

	MeshBuilder& addTexture(Texture* texture, bool copy = false);
	MeshBuilder& addTextures(std::vector<Texture*>& textures, bool copy = false);

	MeshBuilder& enableAttribute(LayoutAttribute attribute);
	MeshBuilder& disableAttribute(LayoutAttribute attribute);

	ModelBuilder& getModelBuilder() const;
	Mesh* build();

	static MeshBuilder& builder();
	
	/** Destructor */
	~MeshBuilder() = default;
private:
	friend class ModelBuilder;
	/** Constructor */
	MeshBuilder();
	void setModelBuilder(ModelBuilder* modelBuilder);

private:

	size_t m_numOfVertices = 0;
	std::shared_ptr<std::vector<glm::vec3>> m_positions = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> m_normals = nullptr;
	std::shared_ptr<std::vector<glm::vec2>> m_texCoords = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> m_colors = nullptr;
	std::shared_ptr<std::vector<unsigned int>> m_indices = nullptr;
	std::shared_ptr<std::vector<std::shared_ptr<Texture>>> m_textures = nullptr;
	VertexLayout m_layout;

	ModelBuilder* m_modelBuilder = nullptr;

};

//void MeshBuilder::setModelBuilder(ModelBuilder<Model>* modelBuilder)
//{
//	m_modelBuilder = modelBuilder;
//}
//
//ModelBuilder<Model>& MeshBuilder::getModelBuilder()
//{
//	return *m_modelBuilder;
//}

#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <memory>

#include "Core.h"

#include "Mesh.h"

class Shader;
class Model;

class EngineAPI ModelBuilder
{
public:
	/** Constructor */
	ModelBuilder() = default;

	/** Destructor */
	~ModelBuilder() = default;

	ModelBuilder& setNumOfVertices(size_t size);

	ModelBuilder& setPositions(std::vector<glm::vec3>& positions, bool copy = false);
	ModelBuilder& setPositions(const float* positions, size_t size);

	ModelBuilder& setNormals(std::vector<glm::vec3>& normals, bool copy = false);
	ModelBuilder& setNormals(const float* normals, size_t size);

	ModelBuilder& setTexcoords(std::vector<glm::vec2>& texCoords, bool copy = false);
	ModelBuilder& setTexcoords(const float* texCoords, size_t size);

	ModelBuilder& setColors(std::vector<glm::vec3>& colors, bool copy = false);

	ModelBuilder& setColors(const float* colors, size_t size);

	ModelBuilder& setIndices(std::vector<unsigned int>& indices, bool copy = false);

	ModelBuilder& setRawVertices(const float* vertices, Mesh::VerticesLayout layout);

	ModelBuilder& setShader(Shader& shader, bool copy = false);

	ModelBuilder& setRawIndices(const unsigned int* indices, size_t size);

	Model* build();
private:
	Mesh* generateMesh();
private:
	size_t m_numOfVertices = 0;
	std::shared_ptr<std::vector<glm::vec3>> m_positions = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> m_normals = nullptr;
	std::shared_ptr<std::vector<glm::vec2>> m_texCoords = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> m_colors = nullptr;
	std::shared_ptr<std::vector<unsigned int>> m_indices = nullptr;
	std::shared_ptr<Shader> m_shader = nullptr;

	bool isBuilt = false;

};

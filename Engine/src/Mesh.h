#pragma once
#include <memory>
#include <vector>

#include "Core.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "VertexArrayObject.h"

#include "ApplicationConstants.h"

#include "Configurations.h"

class MeshBuilder;

class EngineAPI Mesh
{
public:
	
	struct VerticesLayout
	{
		std::vector<LayoutAttributes> attribs;
		size_t numOfVertices = 0;
	};
	// -------------------- Methods -------------------- //
	Mesh();

	void renderMesh(std::shared_ptr<Shader> shader, std::shared_ptr <IRenderer>renderer);

	void addTexture(std::shared_ptr<Texture> texture);
	void addTextures(std::vector<std::shared_ptr<Texture>> textures);

	inline std::vector<std::shared_ptr<Texture>> getTextures() { return m_textures; };

	//void setRawVertices(float* vertices, VerticesLayout& layout);
	//void setRawIndices(unsigned int* indices, size_t size);
	
	void SetTexturesInShader(std::shared_ptr<Shader>& shader);
	void setNumOfVertices(size_t size);
	void setPositions(std::shared_ptr<std::vector<glm::vec3>> positions);
	void setNormals(std::shared_ptr<std::vector<glm::vec3>> normals);
	void setTexcoords(std::shared_ptr<std::vector<glm::vec2>> texCoords);
	void setIndices(std::shared_ptr<std::vector<unsigned int>> indices);
	void setColors(std::shared_ptr<std::vector<glm::vec3>> colors);
	void build();

	~Mesh();
private:
	void calculateNormals();
	void clearMesh();
private:
	// -------------------- Attributes -------------------- //
	/** Mesh positions */
	std::shared_ptr<std::vector<glm::vec3>>       m_positions;

	/** Mesh normals */
	std::shared_ptr<std::vector<glm::vec3>>       m_normals;

	/** Mesh texCoords */
	std::shared_ptr<std::vector<glm::vec2>>       m_texcoords;

	/** Mesh Indices */
	std::shared_ptr<std::vector<unsigned int>>    m_indices;

	/** Mesh textures */
	std::vector<std::shared_ptr<Texture>>         m_textures;

	/** Mesh colors */
	std::shared_ptr<std::vector<glm::vec3>>         m_colors;

	// render data
	std::shared_ptr<ElementBufferObject>  m_ibo;
	std::shared_ptr < VertexBufferObject> m_vbo;
	std::shared_ptr < VertexArrayObject>  m_vao;

	size_t m_numOfVertices = 0;

	size_t m_indexCount = 0;
};

#pragma once
#include <memory>
#include <vector>

#include "Core.h"
#include "Renderer.h"
#include "Shader.h"
#include "TextureHandler.h"

#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "VertexArrayObject.h"

#include "ApplicationConstants.h"

#include "Configurations.h"
#include "VertexLayout.h"

// Forward declerations
class MeshBuilder;
class Model;

/**
 * The mesh class is used to represent the Egnine's basic mesh object,
 * it can be used to manipulate an existing Mesh data or to generate one yourself.
 * It should be used with the MeshBuilder class.
 */
class EngineAPI Mesh
{
public:
	// -------------------- Methods -------------------- //
	/** Constructor */
	Mesh();

	/**
	 * Renders the mesh to the screen using a speicifed shader and a renderer.
	 * 
	 * \param shader
	 * \param renderer
	 */
	void render(Shader& shader, IRenderer& renderer);

	/**
	 * Sets the number of vertices in the mesh.
	 *
	 * \param size	Number of vertices the mesh contains
	 */
	void setNumOfVertices(size_t size);

	/**
	 * Gets the number of vertices in the mesh.
	 *
	 * \param size	Number of vertices the mesh contains
	 */
	size_t getNumOfVertices() const;

	/**
	 * Sets The mesh's positions vertices.
	 *
	 * \param positions
	 */
	void setPositions(std::shared_ptr<std::vector<glm::vec3>> positions);

	/**
	 * Gets The mesh's positions vertices.
	 *
	 * \return	positions vector pointer
	 */
	const std::vector<glm::vec3>* getPositions() const;

	/**
	 * Sets the mesh's Normals vertices.
	 *
	 * \param normals
	 */
	void setNormals(std::shared_ptr<std::vector<glm::vec3>> normals);

	/**
	 * Gets the mesh's Normals vertices.
	 *
	 * \return	normals vector pointer
	 */
	const std::vector<glm::vec3>* getNormals() const;

	/**
	 * Sets the mesh's TexCoords vertices.
	 *
	 * \param texCoords
	 */
	void setTexcoords(std::shared_ptr<std::vector<glm::vec2>> texCoords);

	/**
	 * Gets the mesh's TexCoords vertices.
	 *
	 * \return	texcoords vector pointer
	 */
	const std::vector<glm::vec2>* getTexcoords() const;

	/**
	 * Sets the mesh's Indices.
	 *
	 * \param indices
	 */
	void setIndices(std::shared_ptr<std::vector<unsigned int>> indices);

	/**
	 * Gets the mesh's Indices.
	 *
	 * \return	indices vector pointer
	 */
	const std::vector<unsigned int>* getIndices() const;

	/**
	 * Sets the mesh's Color vertices.
	 *
	 * \param colors
	 */
	void setColors(std::shared_ptr<std::vector<glm::vec3>> colors);

	/**
	 * Gets the mesh's Color vertices.
	 *
	 * \param colors
	 */
	const std::vector<glm::vec3>* getColors() const;

	/**
	 * Clear the Mesh entirely.
	 *
	 */
	void clearMesh();

	/**
	 * Sets the mesh's vertexLayout
	 *
	 */
	void setVertexLayout(VertexLayout layout);

	/**
	 * Gets the mesh's vertexLayout
	 *
	 * \return The mesh's Vertex layout
	 */
	VertexLayout getVertexLayout();

	/**
	 * Build the mesh using the specified vertices data.
	 * This call will initialize all the OpenGL related Buffer data
	 *
	 * return true if the model was built succesfully, false otherwise.
	 */
	bool build();

	/** Destructor */
	~Mesh();
private:
	// -------------------- Friends -------------------- //
	friend class Model;
	friend class MeshBuilder;

private:
	// -------------------- Methods -------------------- //
	void calculateNormals();
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

	

	/** Mesh colors */
	std::shared_ptr<std::vector<glm::vec3>>       m_colors;

	// render data
	std::shared_ptr<ElementBufferObject> m_ibo = nullptr;
	std::shared_ptr<VertexBufferObject>  m_vbo = nullptr;
	std::shared_ptr<VertexArrayObject>   m_vao = nullptr;

	size_t m_numOfVertices = 0;
	size_t m_indexCount = 0;
	VertexLayout m_layout;
	bool m_useColors = false;
};

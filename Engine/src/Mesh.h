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
	// -------------------- Structs -------------------- //
	struct VerticesLayout
	{
		std::vector<LayoutAttributes> attribs;
		size_t numOfVertices = 0;
	};
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

	/** Destructor */
	~Mesh();
private:
	// -------------------- Friends -------------------- //
	friend class Model;
	friend class MeshBuilder;

	// -------------------- Methods -------------------- //
	void SetTexturesInShader(Shader& shader);


	std::vector<std::shared_ptr<Texture>> getTextures() const;

	/**
	 * Sets the number of vertices in the mesh.
	 * 
	 * \param size	Number of vertices the mesh contains
	 */
	void setNumOfVertices(size_t size);

	/**
	 * Sets The mesh's positions vertices.
	 * 
	 * \param positions
	 */
	void setPositions(std::shared_ptr<std::vector<glm::vec3>> positions);

	/**
	 * Sets the mesh's Normals vertices.
	 * 
	 * \param normals
	 */
	void setNormals(std::shared_ptr<std::vector<glm::vec3>> normals);

	/**
	 * Sets the mesh's TexCoords vertices.
	 *
	 * \param texCoords
	 */
	void setTexcoords(std::shared_ptr<std::vector<glm::vec2>> texCoords);

	/**
	 * Sets the mesh's Indices.
	 *
	 * \param indices
	 */
	void setIndices(std::shared_ptr<std::vector<unsigned int>> indices);

	/**
	 * Sets the mesh's Color vertices.
	 *
	 * \param colors
	 */
	void setColors(std::shared_ptr<std::vector<glm::vec3>> colors);

	/**
	 * Add a texture to the mesh.
	 *
	 * \param texture	 a shared pointer to a texture
	 */
	void addTexture(const std::shared_ptr<Texture>& texture);

	/**
	 * Add multiple textures to the mesh.
	 *
	 * \param textures
	 */
	void addTextures(const std::vector<std::shared_ptr<Texture>>& textures);

	/**
	 * Add a texture to the mesh.
	 *
	 * \param texture	a texture raw pointer
	 */
	void addTexture(Texture* texture);

	/**
	 * Add multiple textures to the mesh.
	 *
	 * \param textures
	 */
	void addTextures(std::vector<Texture*>& textures);

	/**
	 * Build the mesh using the specified vertices data.
	 * This call will initialize all the OpenGL related Buffer data
	 * 
	 */
	void build();

	void calculateNormals();

	/**
	 * Clear the Mesh entirely.
	 * 
	 */
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
	std::shared_ptr<ElementBufferObject>  m_ibo = nullptr;
	std::shared_ptr<VertexBufferObject> m_vbo = nullptr;
	std::shared_ptr<VertexArrayObject>  m_vao = nullptr;

	size_t m_numOfVertices = 0;

	size_t m_indexCount = 0;
};

#pragma once

#include <memory>

#include <GL\glew.h>

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "VertexBufferLayout.h"
#include "VertexArrayObject.h"

#include "ApplicationConstants.h"

class Mesh
{
public:
	Mesh(std::shared_ptr<std::vector<Vertex>> vertices, std::shared_ptr<std::vector<unsigned int>> indices);
	Mesh(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize);
	Mesh(std::shared_ptr<std::vector<Vertex>> vertices);
	Mesh(float* vertices, size_t verticesSize);

	void RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr < IRenderer >renderer);

	void AddTexture(std::shared_ptr<Texture> texture);
	void AddTextures(std::vector<std::shared_ptr<Texture>> textures);

	inline std::vector<std::shared_ptr<Texture>> GetTextures() { return m_textures; };

	void SetTexturesInShader(std::shared_ptr<Shader>& shader);

	~Mesh();
private:
	void ClearMesh();
private:
	// mesh data
	std::shared_ptr<std::vector<Vertex>>       m_vertices;
	std::shared_ptr<std::vector<unsigned int>> m_indices;
	std::vector<std::shared_ptr<Texture>>      m_textures;

	// render data
	std::shared_ptr<ElementBufferObject>  m_ibo;
	std::shared_ptr < VertexBufferObject> m_vbo;
	std::shared_ptr < VertexArrayObject>  m_vao;

	GLsizei m_indexCount = 0;
};

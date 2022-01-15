#pragma once

#include <memory>

#include <GL\glew.h>

#include "Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Shader/Texture.h"

#include "Renderer/Buffers/VertexBufferObject.h"
#include "Renderer/Buffers/ElementBufferObject.h"
#include "Renderer/Buffers/VertexBufferLayout.h"
#include "Renderer/Buffers/VertexArrayObject.h"

#include "ApplicationConstants.h"

class Mesh
{
public:
	Mesh(std::shared_ptr<std::vector<Vertex>> vertices, std::shared_ptr<std::vector<unsigned int>> indices, std::vector<std::shared_ptr<Texture>> textures);

	void RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr < Renderer >renderer);

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

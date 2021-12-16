#pragma once

#include <memory>

#include <GL\glew.h>

#include "Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"

#include "Renderer/Buffers/VertexBufferObjectWrapper.h"
#include "Renderer/Buffers/ElementBufferObjectWrapper.h"
#include "Renderer/Buffers/VertexBufferLayout.h"
#include "Renderer/Buffers/VertexArrayObjectWrapper.h"

class Mesh
{  
public:
	Mesh();
	Mesh(const Shader &m_shader, const Renderer& m_renderer);
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOf);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	std::shared_ptr<ElementBufferObjectWrapper> m_ibo;
	std::shared_ptr < VertexBufferObjectWrapper> m_vbo;
	std::shared_ptr < VertexArrayObjectWrapper> m_vao;

	GLsizei m_indexCount = 0;

	const Shader *m_shader;
	const Renderer *m_renderer;
};


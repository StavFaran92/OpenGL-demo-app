#pragma once

#include <GL\glew.h>

#include "Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"

#include "Renderer/Buffers/VertexBuffer.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "Renderer/Buffers/VertexBufferLayout.h"
#include "Renderer/Buffers/VertexArray.h"

class Mesh
{  
public:
	Mesh();
	Mesh(const Shader &shader, const Renderer& renderer);
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOf);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	IndexBuffer *ibo;
	VertexBuffer *vbo;
	VertexArray *vao;
	GLsizei indexCount;

	const Shader *shader;
	const Renderer *renderer;
};


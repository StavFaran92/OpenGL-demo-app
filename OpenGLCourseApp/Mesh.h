#pragma once

#include <GL\glew.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

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


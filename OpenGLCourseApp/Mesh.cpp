
#include "Mesh.h"


Mesh::Mesh() {}

Mesh::Mesh(const Shader &shader, const Renderer &renderer):
	shader(&shader), renderer(&renderer)
{
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	vao = new VertexArray();
	ibo = new IndexBuffer(indices, numOfIndices);
	vbo = new VertexBuffer(vertices, sizeof(vertices[0]) * numOfVertices);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);
}

void Mesh::RenderMesh() 
{

	renderer->Draw(*vao, *ibo, *shader);

	//if (vao && ibo) {
	//	vao->Bind();
	//	ibo->Bind();

	//	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	//	ibo->Unbind();
	//	vao->Unbind();
	//}
}
void Mesh::ClearMesh() 
{

	if (ibo) {

		delete ibo;
		ibo = 0;
	}

	if (vbo) {

		delete vbo;
		vbo = 0;
	}

	if (vao) {

		delete vao;
		vao = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh() {

	ClearMesh();
}
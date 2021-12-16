#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(const Shader &m_shader, const Renderer &m_renderer):
	m_shader(&m_shader), m_renderer(&m_renderer) {}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	m_vao = std::make_shared<VertexArrayObjectWrapper>();
	m_ibo = std::make_shared <ElementBufferObjectWrapper>(indices, numOfIndices);
	m_vbo = std::make_shared <VertexBufferObjectWrapper>(vertices, sizeof(vertices[0]) * numOfVertices);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	m_vao->AttachBuffer(*m_vbo, *m_ibo, layout);
}

void Mesh::RenderMesh() 
{
	m_renderer->Draw(*m_vao, *m_shader);
}
void Mesh::ClearMesh() 
{
	m_vao = nullptr;
	m_ibo = nullptr;
	m_vbo = nullptr;
}

Mesh::~Mesh() {

	ClearMesh();
}
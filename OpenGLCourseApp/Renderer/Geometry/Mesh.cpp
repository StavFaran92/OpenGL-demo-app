#include "Mesh.h"

Mesh::Mesh(Vertex* vertices, int numOfVertices,
	unsigned int* indices, int numOfIndices,
	Texture* textures, int numOfTextures) :
	m_vertices(vertices), m_indices(indices), m_textures(textures)
{
	m_vao = std::make_shared<VertexArrayObjectWrapper>();
	m_ibo = std::make_shared<ElementBufferObjectWrapper>(m_indices, numOfIndices);
	m_vbo = std::make_shared<VertexBufferObjectWrapper>(vertices, sizeof(vertices[0]) * numOfVertices);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	m_vao->AttachBuffer(*m_vbo, *m_ibo, layout);
}

//void Mesh::CreateMesh()
//{
//
//}

void Mesh::RenderMesh(const Shader& shader, const Renderer& renderer)
{
	renderer.Draw(*m_vao, shader);
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
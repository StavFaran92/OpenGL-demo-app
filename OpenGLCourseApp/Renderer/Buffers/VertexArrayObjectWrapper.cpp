#include "VertexArrayObjectWrapper.h"

VertexArrayObjectWrapper::VertexArrayObjectWrapper()
{
	glGenVertexArrays(1, &m_id);
}

VertexArrayObjectWrapper::~VertexArrayObjectWrapper()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArrayObjectWrapper::AttachBuffer(const VertexBufferObjectWrapper& vbo, const ElementBufferObjectWrapper& ebo)
{
	// Bind this VAO
	Bind();

	// Bind VBO to associate with this VAO
	vbo.Bind();

	// Bind IBO to associate with this VAO
	ebo.Bind();
	m_indexCount = ebo.getLength();

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
}

void VertexArrayObjectWrapper::Bind() const
{
	glBindVertexArray(m_id);
}

void VertexArrayObjectWrapper::Unbind() const
{
	glBindVertexArray(0);
}

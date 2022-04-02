#include "VertexArrayObject.h"

#include <gl/glew.h>

#include "Logger.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);
}

VertexArrayObject::~VertexArrayObject()
{
	logInfo( __FUNCTION__ );
	glDeleteVertexArrays(1, &m_id);
}

void VertexArrayObject::AttachBuffer(const VertexBufferObject& vbo, const ElementBufferObject& ebo)
{
	// bind this VAO
	Bind();

	// bind VBO to associate with this VAO
	vbo.Bind();
	m_verticesCount = vbo.getLength();

	// bind IBO to associate with this VAO
	ebo.Bind();
	m_indexCount = ebo.getLength();

	FillVertexAttrib();
}

void VertexArrayObject::FillVertexAttrib()
{
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

void VertexArrayObject::AttachBuffer(const VertexBufferObject& vbo)
{
	// bind this VAO
	Bind();

	// bind VBO to associate with this VAO
	vbo.Bind();
	m_verticesCount = vbo.getLength();

	FillVertexAttrib();
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(m_id);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}
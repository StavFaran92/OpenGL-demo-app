#include "VertexBufferObjectWrapper.h"

#include <GL/glew.h>

VertexBufferObjectWrapper::VertexBufferObjectWrapper(const void* data, unsigned int size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), data, GL_STATIC_DRAW);
}

VertexBufferObjectWrapper::~VertexBufferObjectWrapper()
{
	logInfo( __FUNCTION__ );
	glDeleteBuffers(1, &m_id);
}

void VertexBufferObjectWrapper::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBufferObjectWrapper::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
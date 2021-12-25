#include "ElementBufferObjectWrapper.h"

#include <GL/glew.h>

ElementBufferObjectWrapper::ElementBufferObjectWrapper(unsigned int* data, unsigned int length)
	:m_length(length)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBufferObjectWrapper::~ElementBufferObjectWrapper()
{
	std::cout << __FUNCTION__ << std::endl;
	glDeleteBuffers(1, &m_id);
}

void ElementBufferObjectWrapper::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBufferObjectWrapper::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int ElementBufferObjectWrapper::getLength() const
{
	return m_length;
}
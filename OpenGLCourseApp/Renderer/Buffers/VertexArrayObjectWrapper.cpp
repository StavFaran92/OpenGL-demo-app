#include "VertexArrayObjectWrapper.h"

VertexArrayObjectWrapper::VertexArrayObjectWrapper()
{
	glGenVertexArrays(1, &m_id);
}

VertexArrayObjectWrapper::~VertexArrayObjectWrapper()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArrayObjectWrapper::AttachBuffer(const VertexBufferObjectWrapper& vbo, const ElementBufferObjectWrapper& ebo, const VertexBufferLayout & layout)
{
	// Bind this VAO
	Bind();

	// Bind VBO to associate with this VAO
	vbo.Bind();

	// Bind IBO to associate with this VAO
	ebo.Bind();
	m_indexCount = ebo.getLength();

	// Set vertex attribute pointers
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArrayObjectWrapper::Bind() const
{
	glBindVertexArray(m_id);
}

void VertexArrayObjectWrapper::Unbind() const
{
	glBindVertexArray(0);
}

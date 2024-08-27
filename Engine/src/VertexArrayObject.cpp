#include "VertexArrayObject.h"

#include <gl/glew.h>

#include "Logger.h"
#include "VertexLayout.h"

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

void VertexArrayObject::AttachBuffer(const VertexBufferObject& vbo, const ElementBufferObject* ebo, const VertexLayout& layout)
{
	// bind this VAO
	Bind();

	// bind VBO to associate with this VAO
	vbo.Bind();
	m_verticesCount = vbo.getLength();

	// bind IBO to associate with this VAO
	if (ebo)
	{
		ebo->Bind();
		m_indexCount = ebo->getLength();
	}

	fillVertexAttributes(layout);
}

void VertexArrayObject::fillVertexAttributes(const VertexLayout& layout)
{
	int offset = 0;
	for (auto entry : layout.attribs)
	{
		auto& attribData = getAttributeData(entry);
		glEnableVertexAttribArray(attribData.location);
		if (attribData.typeName == typeid(float).name())
		{
			glVertexAttribPointer(attribData.location, attribData.length, GL_FLOAT, GL_FALSE, layout.stride, (void*)offset);
		}
		else if (attribData.typeName == typeid(int).name())
		{
			glVertexAttribIPointer(attribData.location, attribData.length, GL_INT, layout.stride, (void*)offset);
		}

		offset += attribData.size * attribData.length;
	}
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(m_id);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

unsigned int VertexArrayObject::GetIndexCount() const
{
	return m_indexCount;
}

unsigned int VertexArrayObject::GetVerticesCount() const
{
	return m_verticesCount;
}
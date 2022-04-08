#pragma once
#include "VertexBufferObject.h"

//#include "VertexBufferLayout.h"
#include "VertexBufferElement.h"
#include "ElementBufferObject.h"

class VertexLayout;

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void AttachBuffer(const VertexBufferObject& vbo, const ElementBufferObject* ebo, const VertexLayout& layout);
	void fillVertexAttributes(const VertexLayout& layout);

	void Bind() const;
	void Unbind() const;
	unsigned int GetIndexCount() const;
	unsigned int GetVerticesCount() const;
private:
	unsigned int m_id = 0;
	unsigned int m_indexCount = 0;
	unsigned int m_verticesCount = 0;
};

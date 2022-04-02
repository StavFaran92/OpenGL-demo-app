#pragma once
#include "VertexBufferObject.h"

//#include "VertexBufferLayout.h"
#include "VertexBufferElement.h"
#include "ElementBufferObject.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void AttachBuffer(const VertexBufferObject& vbo, const ElementBufferObject& ebo);
	void FillVertexAttrib();
	void AttachBuffer(const VertexBufferObject& vbo);

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetIndexCount() const { return m_indexCount; }
	inline unsigned int GetVerticesCount() const { return m_verticesCount; }
private:
	unsigned int m_id = 0;
	unsigned int m_indexCount = 0;
	unsigned int m_verticesCount = 0;
};

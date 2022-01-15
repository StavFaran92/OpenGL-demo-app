#pragma once
#include "VertexBufferObject.h"

#include "VertexBufferLayout.h"
#include "VertexBufferElement.h"
#include "ElementBufferObject.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void AttachBuffer(const VertexBufferObject& vbo, const ElementBufferObject& ebo);

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetIndexCount() const { return m_indexCount; }
private:
	unsigned int m_id;
	unsigned int m_indexCount;
};

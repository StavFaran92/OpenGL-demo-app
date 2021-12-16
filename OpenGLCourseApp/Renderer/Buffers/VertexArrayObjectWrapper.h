#pragma once
#include "VertexBufferObjectWrapper.h"

#include "VertexBufferLayout.h"
#include "VertexBufferElement.h"
#include "IndexBuffer.h"

class VertexArrayObjectWrapper
{
public:
	VertexArrayObjectWrapper();
	~VertexArrayObjectWrapper();

	void AttachBuffer(const VertexBufferObjectWrapper& vbo, const IndexBuffer& ibo, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetIndexCount() const { return m_indexCount; }
private:
	unsigned int m_id;
	unsigned int m_indexCount;
};


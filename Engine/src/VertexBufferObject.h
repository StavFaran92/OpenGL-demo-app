#pragma once

#include <iostream>

#include "Logger.h"



#include "Vertex.h"

class VertexBufferObject
{
public:
	VertexBufferObject(const void* data, unsigned int size);
	~VertexBufferObject();

	void Bind() const;
	void Unbind() const;

	unsigned int getLength() const;
private:
	unsigned int m_id = 0;
	unsigned int m_length = 0;
};

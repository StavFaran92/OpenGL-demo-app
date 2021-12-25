#pragma once

#include <iostream>

#include "glm/glm.hpp"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class VertexBufferObjectWrapper
{
public:
	VertexBufferObjectWrapper(const void* data, unsigned int size);
	~VertexBufferObjectWrapper();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_id;
};

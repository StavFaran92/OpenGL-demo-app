#pragma once

#include <iostream>

#include "Utils/Logger/Logger.h"

#include "glm/glm.hpp"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class VertexBufferObject
{
public:
	VertexBufferObject(const void* data, unsigned int size);
	~VertexBufferObject();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_id;
};

#pragma once
#include "Renderer/Buffers/VertexArray.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "Renderer/Shader/Shader.h"

class Renderer
{
public:
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	void Clear() const;
};


#pragma once
#include "Renderer/Buffers/VertexArrayObjectWrapper.h"
#include "Renderer/Shader/Shader.h"

class Renderer
{
public:
	void Draw(const VertexArrayObjectWrapper& vao, const Shader& m_shader) const;
	void Clear() const;
};

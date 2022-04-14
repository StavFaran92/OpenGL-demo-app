#pragma once

#include "IRenderer.h"

class Renderer2D : public IRenderer
{
public:
	Renderer2D() = default;
	// Inherited via IRenderer
	virtual void draw(const VertexArrayObject& vao, Shader& shader) const override;
	virtual void Clear() const override;
};


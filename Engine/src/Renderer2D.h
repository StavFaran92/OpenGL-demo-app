#pragma once

#include "IRenderer.h"

class Renderer2D : public IRenderer
{
public:
	Renderer2D() = default;
	// Inherited via IRenderer
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
	void Clear() const override;

	// Inherited via IRenderer
	glm::mat4 getProjection() const override;
};


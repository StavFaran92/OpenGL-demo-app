#pragma once

#include "IRenderer.h"

#include "VertexArrayObject.h"

class EngineAPI RendererIntermediate : public IRenderer
{
public:
	void clear() const override;
protected:
	void draw(const VertexArrayObject& vao) const override;

};


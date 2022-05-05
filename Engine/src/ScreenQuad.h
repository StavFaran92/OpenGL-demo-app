#pragma once

#include "Quad.h"

template<typename T> class ObjectHandler;

class ScreenQuad : public Quad
{
public:
	static ObjectHandler<ScreenQuad> GenerateScreenQuad();
	void draw(IRenderer& renderer, Shader* shader = nullptr) override;
};


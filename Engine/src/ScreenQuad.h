#pragma once

#include "Quad.h"

class ScreenQuad : public Quad
{
public:
	static std::shared_ptr<ScreenQuad> GenerateScreenQuad();
	void draw(IRenderer& renderer, Shader* shader = nullptr) override;
};


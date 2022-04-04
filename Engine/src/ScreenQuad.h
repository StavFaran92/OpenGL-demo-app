#pragma once

#include "Quad.h"

class ScreenQuad : public Quad
{
public:
	static std::shared_ptr<ScreenQuad> GenerateScreenQuad();
	void Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader = nullptr) override;
};


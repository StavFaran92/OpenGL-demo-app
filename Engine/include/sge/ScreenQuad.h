#pragma once

#include "Model.h"

class ScreenQuad : Model
{
public:
	static std::shared_ptr<ScreenQuad> GenerateScreenQuad();
	void Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader = nullptr) override;
};


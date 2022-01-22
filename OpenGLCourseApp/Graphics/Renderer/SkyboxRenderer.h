#pragma once

#include "Graphics/Renderer/Renderer.h"

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer(const Renderer& other) : Renderer(other) {};

	void SetMVP(std::shared_ptr<Shader>& shader) const override;
};


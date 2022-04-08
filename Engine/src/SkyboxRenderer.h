#pragma once

#include "Renderer.h"

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer(const Renderer& other) : Renderer(other) {};

	void SetMVP(Shader& shader) const override;
};


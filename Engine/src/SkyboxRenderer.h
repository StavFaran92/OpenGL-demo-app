#pragma once

#include "Renderer.h"

class Material;

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer() = default;
	SkyboxRenderer(const Renderer& other) : Renderer(other) {};

	void SetMVP(Shader& shader) const override;
	void render(const DrawQueueRenderParams& renderParams);
};


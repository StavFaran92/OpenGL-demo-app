#pragma once

#include "Graphics/Renderer/Renderer.h"

class SkyboxRenderer : public Renderer
{
	SkyboxRenderer( std::shared_ptr<Renderer> other);

	void SetMVP(std::shared_ptr<Shader>& shader) const override;
};


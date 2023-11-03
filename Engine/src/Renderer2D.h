#pragma once

#include "RendererIntermediate.h"

class Model;
class ICamera;

class Renderer2D : public RendererIntermediate
{
public:
	Renderer2D() = default;

	// Inherited via IRenderer
	virtual void render(const DrawQueueRenderParams& renderParams) override;

	// Inherited via IRenderer
	void renderScene(DrawQueueRenderParams& renderParams) override;
};


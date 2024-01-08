#pragma once
#include "RendererIntermediate.h"

#include "Core.h"

// forward declerations
class ICamera;
class SkyboxRenderer;
class Model;
class Mesh;
class Transformation;
class Shader;
class Entity;
class Mesh;


class EngineAPI Renderer : public RendererIntermediate
{
public:
	// Constructor
	Renderer(Scene* scene);

	bool init() override;

	void render(const IRenderer::DrawQueueRenderParams& renderParams);

	void enableWireframeMode(bool enable);
	// Inherited via IRenderer
	void renderScene(DrawQueueRenderParams& renderParams) override;

private:
	void setUniforms(const DrawQueueRenderParams& renderParams);
protected:
	bool m_wireFrameMode = false;

	Scene* m_scene = nullptr;
};

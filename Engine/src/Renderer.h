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
	Renderer();

	bool init() override;

	void render(const IRenderer::DrawQueueRenderParams& renderParams);

	void enableWireframeMode(bool enable);
	// Inherited via IRenderer
	void renderScene(DrawQueueRenderParams& renderParams) override;

private:
	void setUniforms(const DrawQueueRenderParams& renderParams);
protected:
	glm::mat4 m_projection;

	bool m_wireFrameMode = false;





};

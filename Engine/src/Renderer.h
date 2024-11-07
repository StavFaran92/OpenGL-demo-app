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
	Renderer(std::shared_ptr<FrameBufferObject> renderTarget, Scene* scene);

	bool init() override;

	void render();

	void enableWireframeMode(bool enable);
	// Inherited via IRenderer
	void renderScene(Scene*) override;

	uint32_t getRenderTarget() const override;

private:
	void setUniforms();
protected:
	bool m_wireFrameMode = false;

	Scene* m_scene = nullptr;

	std::shared_ptr<FrameBufferObject> m_renderTargetFBO;
};

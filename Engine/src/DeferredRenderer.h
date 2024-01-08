#pragma once

#include "RendererIntermediate.h"

#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"
#include "Entity.h"

class FrameBufferObject;
class RenderBufferObject;

class DeferredRenderer : public RendererIntermediate
{
public:
	DeferredRenderer(Scene* scene);
	// Inherited via IRenderer
	bool init() override;
	void render(const DrawQueueRenderParams& renderParams) override;
	void renderScene(DrawQueueRenderParams& renderParams) override;

	const FrameBufferObject& getGBuffer() const;

private:
	bool setupGBuffer();

private:
	FrameBufferObject m_gBuffer;
	
	RenderBufferObject m_renderBuffer;
	
	TextureHandler* m_positionTexture = nullptr;
	TextureHandler* m_normalTexture = nullptr;
	TextureHandler* m_albedoSpecularTexture = nullptr;
	
	Scene* m_scene = nullptr;
	
	
	std::shared_ptr<Shader> m_gBufferShader;
	std::shared_ptr<Shader> m_lightPassShader;
};


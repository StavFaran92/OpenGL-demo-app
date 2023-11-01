#pragma once

#include "IRenderer.h"

#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"
#include "Entity.h"

class FrameBufferObject;
class RenderBufferObject;

class DeferredRenderer : public IRenderer
{
	DeferredRenderer(Scene* scene);
	// Inherited via IRenderer
	bool init() override;
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
	void clear() const override;
	glm::mat4 getProjection() const override;
	void render(const DrawQueueRenderParams& renderParams) override;
	void renderScene(DrawQueueRenderParams& renderParams) override;

private:
	FrameBufferObject m_gBuffer;
	RenderBufferObject m_renderBuffer;
	TextureHandler* m_positionTexture = nullptr;
	TextureHandler* m_normalTexture = nullptr;
	TextureHandler* m_albedoSpecularTexture = nullptr;
	Entity m_quad;
	Scene* m_scene = nullptr;
	std::shared_ptr<IRenderer> m_2DRenderer;
	std::shared_ptr<Shader> m_screenShader;
};


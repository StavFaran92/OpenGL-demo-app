#pragma once

#include "IRenderer.h"

#include "TextureHandler.h"

class FrameBufferObject;

class DeferredRenderer : public IRenderer
{
	// Inherited via IRenderer
	bool init() override;
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
	void clear() const override;
	glm::mat4 getProjection() const override;
	void render(const DrawQueueRenderParams& renderParams) override;
	void renderScene(DrawQueueRenderParams& renderParams) override;

private:
	FrameBufferObject m_gBuffer;
	TextureHandler* m_positionTexture = nullptr;
	TextureHandler* m_normalTexture = nullptr;
	TextureHandler* m_albedoSpecularTexture = nullptr;
};


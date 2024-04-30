#pragma once

#include "RendererIntermediate.h"

#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "Resource.h"
#include "Entity.h"
#include "Configurations.h"

class FrameBufferObject;
class RenderBufferObject;

class DeferredRenderer : public RendererIntermediate
{
public:
	DeferredRenderer(std::shared_ptr<FrameBufferObject> renderTarget, Scene* scene);
	// Inherited via IRenderer
	bool init() override;
	void render(const DrawQueueRenderParams& renderParams) override;
	void renderScene(DrawQueueRenderParams& renderParams) override;
	uint32_t getRenderTarget() const override;

	const FrameBufferObject& getGBuffer() const;

private:
	bool setupGBuffer();
	bool setupSSAO();

private:
	FrameBufferObject m_gBuffer;
	
	RenderBufferObject m_renderBuffer{ SCREEN_WIDTH,SCREEN_HEIGHT };
	
	Resource<Texture> m_positionTexture = nullptr;
	Resource<Texture> m_normalTexture = nullptr;
	Resource<Texture> m_albedoTexture = nullptr;
	Resource<Texture> m_MRATexture = nullptr;
	
	Scene* m_scene = nullptr;

	std::shared_ptr<FrameBufferObject> m_renderTargetFBO;

	Entity m_quad;
	std::shared_ptr<Shader> m_screenShader;
	
	
	std::shared_ptr<Shader> m_gBufferShader;
	std::shared_ptr<Shader> m_lightPassShader;

	// SSAO
	FrameBufferObject m_ssaoFBO;
	RenderBufferObject m_ssaoRenderBuffer{ SCREEN_WIDTH,SCREEN_HEIGHT };
	Resource<Texture> m_ssaoNoiseTexture = nullptr;
	Resource<Texture> m_ssaoColorBuffer = nullptr;
	std::shared_ptr<Shader> m_ssaoPassShader;
	std::vector<glm::vec3> m_ssaoKernel;
	
	FrameBufferObject m_ssaoBlurFBO;
	RenderBufferObject m_ssaoBlurRenderBuffer{ SCREEN_WIDTH,SCREEN_HEIGHT };
	Resource<Texture> m_ssaoBlurColorBuffer = nullptr;
	std::shared_ptr<Shader> m_ssaoBlurPassShader;

};


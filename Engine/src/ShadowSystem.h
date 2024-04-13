#pragma once

#include "Core.h"

#include "FrameBufferObject.h"
#include "IRenderer.h"

class Shader;
class Scene;
class ScreenBufferDisplay;
class TextureHandler;

class EngineAPI ShadowSystem
{
public:
	enum class ShadowAlgorithm
	{
		ShadowMapping,
		CSM
	};

	ShadowSystem(Context* context, Scene* scene);

	bool init();

	void renderToDepthMap(const IRenderer::DrawQueueRenderParams* params);

	TextureHandler* getShadowMap() const;
private:
	bool m_isInit = false;

	ShadowAlgorithm m_shadowAlgorithm = ShadowAlgorithm::ShadowMapping;

	FrameBufferObject m_fbo;
	Scene* m_scene = nullptr;
	std::shared_ptr<Shader> m_simpleDepthShader = nullptr;
	TextureHandler* m_depthMapTexture = nullptr;

	std::shared_ptr<ScreenBufferDisplay> m_bufferDisplay;

	Context* m_context = nullptr;
};
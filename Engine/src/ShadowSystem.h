#pragma once

#include "Core.h"

#include "FrameBufferObject.h"
#include "IRenderer.h"

class Shader;
class Scene;

class EngineAPI ShadowSystem
{
public:
	enum class ShadowAlgorithm
	{
		ShadowMapping,
		CSM
	};

	bool init(Scene* scene);

	void renderToDepthMap(const IRenderer::DrawQueueRenderParams* params);
private:
	bool m_isInit = false;

	ShadowAlgorithm m_shadowAlgorithm = ShadowAlgorithm::ShadowMapping;

	FrameBufferObject m_fbo;
	Scene* m_scene = nullptr;
	std::shared_ptr<Shader> m_simpleDepthShader = nullptr;
};
#pragma once

#include "Core.h"

#include "FrameBufferObject.h"

class Scene;
class IRenderer;

class EngineAPI ShadowSystem
{
public:
	enum class ShadowAlgorithm
	{
		ShadowMapping,
		CSM
	};

	bool init(Scene* scene);

	void renderToDepthMap(const IRenderer::Params* params);
private:
	bool m_isInit = false;

	ShadowAlgorithm m_shadowAlgorithm = ShadowAlgorithm::ShadowMapping;

	FrameBufferObject m_fbo;
	Scene* m_scene = nullptr;
};
#pragma once

#include <memory>

#include "IRenderer.h"

class UniformBufferObject;
class Context;
class Scene;

class LightSystem
{
public:
	LightSystem(Context* context, Scene* scene);
	bool init();
private:
	void setLightsInUBO(const IRenderer::DrawQueueRenderParams* params);
private:
	bool m_isInit = false;

	std::shared_ptr<UniformBufferObject> m_uboLights;
};


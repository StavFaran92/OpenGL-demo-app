#include "LightSystem.h"

#include "UniformBufferObject.h"
#include "Scene.h"
#include "Context.h"
#include "IRenderer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Transformation.h"

struct PointLight
{
	glm::vec4 position;
	glm::vec4 color;
};

struct DirLight
{
	glm::vec4 direction;
	glm::vec4 color;
};

LightSystem::LightSystem(Context* context, Scene* scene)
{
}

bool LightSystem::init()
{
	m_uboLights = std::make_shared<UniformBufferObject>(sizeof(PointLight) * 16 + sizeof(DirLight) * 16 + sizeof(int) * 2);
	m_uboLights->attachToBindPoint(1);

    return false;
}

void LightSystem::setLightsInUBO(const IRenderer::DrawQueueRenderParams* params)
{
	{
		// Use all point lights
		auto view = params->registry->view<DirectionalLight>();

		int i = 0;
		for (auto it = view.begin(); it != view.end(); ++it, ++i)
		{
			auto& pLight = view.get<DirectionalLight>(*it);
			pLight.useLight(*shader, i);
		}

		m_uboLights->setData(0, sizeof(float), &elapsed);
	}

	{
		// Use all point lights
		auto view = params->registry->view<PointLight, Transformation>();

		int i = 0;
		for (auto it = view.begin(); it != view.end(); ++it, ++i)
		{
			auto& pLight = view.get<PointLight>(*it);
			auto& transform = view.get<Transformation>(*it);
			shader->setValue("pointLights[" + std::to_string(i) + "]" + ".position", transform.getLocalPosition());
			pLight.useLight(*shader, i);
		}
		shader->setValue("pointLightCount", i);
	}
}

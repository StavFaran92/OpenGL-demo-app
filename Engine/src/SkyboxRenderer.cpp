#include "SkyboxRenderer.h"
#include "Shader.h"
#include "ICamera.h"

void SkyboxRenderer::SetMVP(Shader& shader) const
{
	shader.setValue("projection", m_projection);
	shader.setValue("view", glm::mat4(glm::mat3(m_camera->getView())));
}

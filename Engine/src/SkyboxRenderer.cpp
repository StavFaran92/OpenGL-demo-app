#include "SkyboxRenderer.h"
#include "Shader.h"
#include "ICamera.h"

void SkyboxRenderer::SetMVP(Shader& shader) const
{
	shader.setMat4("projection", m_projection);
	shader.setMat4("view", glm::mat4(glm::mat3(m_camera->getView())));
}

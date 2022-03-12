#include "SkyboxRenderer.h"
#include "Shader.h"
#include "ICamera.h"

void SkyboxRenderer::SetMVP(std::shared_ptr<Shader>& shader) const
{
	shader->SetMat4("projection", m_projection);
	shader->SetMat4("view", glm::mat4(glm::mat3(m_camera->getView())));
}

#include "SkyboxRenderer.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Camera/ICamera.h"

SkyboxRenderer::SkyboxRenderer(std::shared_ptr<Renderer> other)
{
	m_defaultShader = other->GetDefaultShader();
	m_camera = other->GetCamera();
	m_projection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);
}

void SkyboxRenderer::SetMVP(std::shared_ptr<Shader>& shader) const
{
	shader->SetMat4("projection", m_projection);
	shader->SetMat4("view", glm::mat4(glm::mat3(m_camera->getView())));
}

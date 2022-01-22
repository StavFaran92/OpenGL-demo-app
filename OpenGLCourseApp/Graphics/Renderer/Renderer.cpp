#include "Renderer.h"

#include "Graphics/Buffers/VertexArrayObject.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Camera/ICamera.h"
#include "Graphics/Camera/EditorCamera.h"

#include "glm/glm.hpp"

Renderer::Renderer()
{
	m_defaultShader = std::make_shared<Shader>("Resources\\Shaders\\shader.vert", "Resources\\Shaders\\shader.frag");
	m_defaultShader->SetEnableLights(true);
	m_defaultShader->SetEnableMaterials(true);
	m_defaultShader->SetEnableTextures(true);
	//m_camera = std::make_shared<FlyCamera>(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 1.0f, .5f);
	m_camera = std::make_shared<EditorCamera>(glm::vec3(5.0f, 5.0f, 5.0f), 1.0f, .5f);
	m_projection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);
}

void Renderer::Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const
{
	SetMVP(shader);

	vao.Bind();

	if (vao.GetIndexCount() == 0)
	{
		glDrawArrays(m_drawType, 0, vao.GetVerticesCount());
	}
	else 
	{
		glDrawElements(m_drawType, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}

}

void Renderer::SetMVP(std::shared_ptr<Shader>& shader) const
{
	shader->SetFloat("viewPos", m_camera->getPosition()); // TODO fix
	shader->SetMat4("projection", m_projection);
	shader->SetMat4("view", glm::mat4(glm::mat3(m_camera->getView())));
}

std::shared_ptr<Shader> Renderer::GetDefaultShader() const
{
	return m_defaultShader;
}

std::shared_ptr<ICamera> Renderer::GetCamera() const
{
	return m_camera;
}

void Renderer::Clear() const
{
	// Clear window
	//glClearStencil(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
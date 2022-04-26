#include "Renderer.h"

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "VertexArrayObject.h"
#include "Shader.h"
#include "ICamera.h"
#include "EditorCamera.h"
#include "SkyboxRenderer.h"
#include "Logger.h"


Renderer::Renderer()
{
	m_phongShader = std::shared_ptr<Shader>(Shader::PhongShader());
	//m_camera = std::make_shared<FlyCamera>(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 1.0f, .5f);
	m_camera = std::make_shared<EditorCamera>(glm::vec3(5.0f, 5.0f, 5.0f), 1.0f, .5f);
	m_projection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);
}

Renderer::Renderer(const Renderer& other)
{
	m_phongShader = other.m_phongShader;
	m_camera = other.m_camera;
	m_projection = other.m_projection;
}

void Renderer::draw(const VertexArrayObject& vao, Shader& shader) const
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

void Renderer::SetMVP(Shader& shader) const
{
	shader.setMat4("projection", m_projection);
	shader.setMat4("view", m_camera->getView());
}

std::shared_ptr<Shader> Renderer::GetDefaultShader() const
{
	return m_phongShader;
}

std::shared_ptr<ICamera> Renderer::GetCamera() const
{
	return m_camera;
}

glm::mat4 Renderer::getProjection() const
{
	return m_projection;
}

void Renderer::Clear() const
{
	// Clear window
	//glClearStencil(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
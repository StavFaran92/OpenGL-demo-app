#include "DebugHelper.h"

#include "VertexArrayObject.h"
#include "VertexLayout.h"
#include "Shader.h"
#include "Graphics.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

DebugHelper::DebugHelper()
{
	void* data = new float[3] {0, 0, 0};
	m_vao = std::make_shared<VertexArrayObject>();
	m_vbo = std::make_shared<VertexBufferObject>(data, 3, sizeof(float) * 3);
	VertexLayout layout;
	layout.attribs.push_back(LayoutAttribute::Positions);
	m_vao->AttachBuffer(*m_vbo.get(), NULL, layout);

	m_shader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/DebugShader.glsl");
}


void DebugHelper::drawPoint(const glm::vec3& pos)
{
	auto graphics = Engine::get()->getSubSystem<Graphics>();

	// Set the size of the point
	glPointSize(10.0f); // Size in pixels

	auto model = glm::translate(glm::mat4(1.0f), pos);

	m_shader->use();
	m_shader->setUniformValue("color", glm::vec3{ 1.0, 0, 0 });
	m_shader->setModelMatrix(model);
	m_shader->setViewMatrix(*graphics->view);
	m_shader->setProjectionMatrix(*graphics->projection);

	m_vao->Bind();

	// Draw the point
	glDrawArrays(GL_POINTS, 0, 1);
}

void DebugHelper::drawLine(const glm::vec3& p1, const glm::vec3& p2)
{
}

void DebugHelper::drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
}

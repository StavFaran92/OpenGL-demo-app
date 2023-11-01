#include "Renderer2D.h"

#include "Logger.h"
#include <GL/glew.h>

#include "VertexArrayObject.h"
#include "ICamera.h"

void Renderer2D::draw(const VertexArrayObject& vao, Shader& shader) const
{
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

void Renderer2D::clear() const
{
}

glm::mat4 Renderer2D::getProjection() const
{
	return glm::mat4(1);
}

//void Renderer2D::render(Model* model, Shader* shader/* = nullptr*/)
//{
//}

void Renderer2D::render(const DrawQueueRenderParams& renderParams)
{
}

void Renderer2D::renderScene(DrawQueueRenderParams& renderParams)
{
}

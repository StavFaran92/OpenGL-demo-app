#include "Renderer2D.h"

#include "Logger.h"
#include <GL/glew.h>

#include "VertexArrayObject.h"

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

void Renderer2D::Clear() const
{
}

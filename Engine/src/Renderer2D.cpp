#include "Renderer2D.h"

void Renderer2D::Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const
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

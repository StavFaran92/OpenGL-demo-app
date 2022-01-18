#include "Renderer2D.h"

void Renderer2D::Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const
{
	vao.Bind();

	if (vao.GetIndexCount() == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, vao.GetVerticesCount());
	}
	else
	{
		glDrawElements(GL_TRIANGLES, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}
}

void Renderer2D::Clear() const
{
}

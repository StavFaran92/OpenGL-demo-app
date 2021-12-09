#include "Renderer.h"

void Renderer::Draw(const VertexArray & vao, const IndexBuffer & ibo, const Shader & shader) const
{
	shader.UseShader();
	vao.Bind();
	ibo.Bind();

	glDrawElements(GL_TRIANGLES, ibo.getLength(), GL_UNSIGNED_INT, 0);
}

void Renderer::Clear() const
{
	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

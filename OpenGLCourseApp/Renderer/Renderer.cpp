#include "Renderer.h"

void Renderer::Draw(const VertexArrayObjectWrapper & vao, const Shader & m_shader) const
{
	// TODO fix, the use shader cannot be here since if we run calculations on the uniforms objects before this call
	// it will not be bound to this shader program.
	//m_shader.UseShader();
	vao.Bind();

	glDrawElements(GL_TRIANGLES, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::Clear() const
{
	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

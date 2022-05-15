#include "GpuInstancingRenderer.h"

#include <gl/glew.h>

#include "Model.h"
#include "Shader.h"
#include "Transformation.h"
#include "VertexArrayObject.h"

void GpuInstancingRenderer::render(Model* model, std::vector<Transformation*>* transformations, Shader* shader)
{
	SetMVP(shader);

	m_amount = transformations->size();
}

void GpuInstancingRenderer::draw(const VertexArrayObject& vao, Shader& shader) const
{
	

	vao.Bind();

	if (vao.GetIndexCount() == 0)
	{
		glDrawArraysInstanced(m_drawType, 0, vao.GetVerticesCount(), m_amount);
	}
	else
	{
		glDrawElementsInstanced(m_drawType, vao.GetIndexCount(), GL_UNSIGNED_INT, 0, m_amount);
	}
}

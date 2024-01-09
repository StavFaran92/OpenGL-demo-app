#include "GpuInstancingRenderer.h"

#include <gl/glew.h>

#include "Shader.h"
#include "Transformation.h"
#include "VertexArrayObject.h"
#include "InstanceBatch.h"

GpuInstancingRenderer::GpuInstancingRenderer(std::shared_ptr<FrameBufferObject> renderTarget, Scene* scene)
	: Renderer(renderTarget, scene)
{
	glGenBuffers(1, &buffer);
}

void GpuInstancingRenderer::render(const DrawQueueRenderParams& renderParams)
{
	auto& batch = renderParams.entity->getComponent<InstanceBatch>();

	m_amount = batch.getAmount();

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, m_amount * sizeof(glm::mat4), &batch.getMatrices()[0], GL_STATIC_DRAW);

	renderParams.shader->setValue("isGpuInstanced", 1);

	Renderer::render(renderParams);
}

void GpuInstancingRenderer::draw(const VertexArrayObject& vao) const
{
	vao.Bind();
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// set attribute pointers for matrix (4 times vec4)
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);

	if (vao.GetIndexCount() == 0)
	{
		glDrawArraysInstanced(m_drawType, 0, vao.GetVerticesCount(), m_amount);
	}
	else
	{
		glDrawElementsInstanced(m_drawType, vao.GetIndexCount(), GL_UNSIGNED_INT, 0, m_amount);
	}

	vao.Unbind();
}

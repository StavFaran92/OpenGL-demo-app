#pragma once

#include <memory>
#include "VertexArrayObject.h"

#include "gl/glew.h"

class RenderCommand
{
public:
	static void draw(const VertexArrayObject* vao)
	{
		vao->Bind();

		if (vao->GetIndexCount() == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, vao->GetVerticesCount());
		}
		else
		{
			glDrawElements(GL_TRIANGLES, vao->GetIndexCount(), GL_UNSIGNED_INT, 0);
		}
	}

	static void drawInstanced(const VertexArrayObject* vao, int count)
	{
		vao->Bind();

		if (vao->GetIndexCount() == 0)
		{
			glDrawArraysInstanced(GL_TRIANGLES, 0, vao->GetVerticesCount(), count);
		}
		else
		{
			glDrawElementsInstanced(GL_TRIANGLES, vao->GetIndexCount(), GL_UNSIGNED_INT, 0, count);
		}
	}
};
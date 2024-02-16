#pragma once

#include <memory>
#include "VertexArrayObject.h"

class RenderCommand
{
public:
	static void drawIndexed(const VertexArrayObject* vao)
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
};
#include "RenderBufferObject.h"

RenderBufferObject::RenderBufferObject()
{
	glGenRenderbuffers(1, &m_id);
}

void RenderBufferObject::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void RenderBufferObject::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

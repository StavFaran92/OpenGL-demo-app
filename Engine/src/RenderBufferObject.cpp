#include "RenderBufferObject.h"

#include "GL/glew.h"

RenderBufferObject::RenderBufferObject()
{
	glGenRenderbuffers(1, &m_id);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768);
	Unbind();
}

void RenderBufferObject::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void RenderBufferObject::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

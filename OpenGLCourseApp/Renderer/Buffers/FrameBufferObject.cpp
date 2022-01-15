#include "FrameBufferObject.h"

#include "GL/glew.h"

FrameBufferObject::FrameBufferObject()
{
	glGenFramebuffers(1, &m_id);
}

void FrameBufferObject::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FrameBufferObject::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBufferObject::IsComplete()
{
	bool result = false;

	Bind();
	result = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	return result;

}

void FrameBufferObject::AttachRenderBuffer(uint32_t renderBufferID, AttachmentType type)
{
	GLint attachment = GL_FRAMEBUFFER_DEFAULT;
	switch (type)
	{
	case AttachmentType::Color:
		attachment = GL_COLOR_ATTACHMENT0;
		break;
	case AttachmentType::Depth:
		attachment = GL_DEPTH_ATTACHMENT;
		break;
	case AttachmentType::Stencil:
		attachment = GL_STENCIL_ATTACHMENT;
		break;
	case AttachmentType::Depth_Stencil:
		attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		break;
	}
	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBufferID);
}

void FrameBufferObject::AttachTexture(uint32_t textureID)
{
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
}

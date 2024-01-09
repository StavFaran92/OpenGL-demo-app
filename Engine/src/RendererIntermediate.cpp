#include "RendererIntermediate.h"

#include <GL/glew.h>

#include "Engine.h"
#include "Window.h"
#include "Logger.h"
#include "ScreenQuad.h"
#include "Renderer2D.h"

void RendererIntermediate::draw(const VertexArrayObject& vao) const
{
	vao.Bind();

	if (vao.GetIndexCount() == 0)
	{
		glDrawArrays(m_drawType, 0, vao.GetVerticesCount());
	}
	else
	{
		glDrawElements(m_drawType, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}
}

uint32_t RendererIntermediate::getRenderTarget() const
{
	return 0;
}

void RendererIntermediate::clear() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//bool RendererIntermediate::setupRenderTarget(Scene* scene)
//{
//	m_renderTargetFBO.bind();
//
//	auto width = Engine::get()->getWindow()->getWidth();
//	auto height = Engine::get()->getWindow()->getHeight();
//
//	// Generate Texture for Position data
//	m_renderTargetTexture = Texture::createEmptyTexture(width, height);
//	m_renderTargetFBO.attachTexture(m_renderTargetTexture->getID(), GL_COLOR_ATTACHMENT0);
//
//	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
//	glDrawBuffers(1, attachments);
//
//	// Create RBO and attach to FBO
//	m_renderTargetFBO.attachRenderBuffer(m_renderTargetRBO.GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);
//
//	if (!m_renderTargetFBO.isComplete())
//	{
//		logError("FBO is not complete!");
//		return false;
//	}
//
//	m_renderTargetFBO.unbind();
//
//	// Generate screen quad
//	m_quad = ScreenQuad::GenerateScreenQuad(scene);
//
//	// Generate screen shader
//	m_screenShader = Shader::createShared<Shader>(
//		"Resources/Engine/Shaders/PostProcess/PostProcessShader_default.vert",
//		"Resources/Engine/Shaders/PostProcess/PostProcessShader_default.frag");
//
//	// Generate screen renderer
//	m_2DRenderer = std::make_shared<Renderer2D>();
//
//	return true;
//}
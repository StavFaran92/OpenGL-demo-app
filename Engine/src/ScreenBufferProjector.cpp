#include "ScreenBufferProjector.h"

#include "Model.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "Renderer2D.h"
#include "ScreenQuad.h"
#include "Engine.h"
#include "Window.h"
#include "Logger.h"
#include "TextureHandler.h"

bool ScreenBufferProjector::init()
{
	m_frameBuffer = std::make_shared<FrameBufferObject>();
	m_frameBuffer->Bind();

	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();
	m_textureHandler = std::shared_ptr<TextureHandler>(Texture::createEmptyTexture(width, height));
	m_frameBuffer->AttachTexture(m_textureHandler->getID());

	m_renderBuffer = std::make_shared<RenderBufferObject>();

	m_frameBuffer->AttachRenderBuffer(m_renderBuffer->GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	if (!m_frameBuffer->IsComplete())
	{
		logError("Framebuffer is not complete!");
		return false;
	}
	m_frameBuffer->Unbind();

	m_quad = ScreenQuad::GenerateScreenQuad();
	m_screenShader = std::make_shared<Shader>("Resources\\Shaders\\PostProcess\\PostProcessShader_default.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_default.frag");
	m_renderer = std::make_shared<Renderer2D>();
}

void ScreenBufferProjector::RedirectToFrameBuffer()
{
	m_frameBuffer->Bind();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void ScreenBufferProjector::RedirectToDefault()
{
	m_frameBuffer->Unbind();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_screenShader->use();

	glDisable(GL_DEPTH_TEST);

	//m_textureHandler->bind();
	glBindTexture(GL_TEXTURE_2D, m_textureHandler->getID());

	m_quad->draw(*m_renderer.get(), m_screenShader.get());
}

void ScreenBufferProjector::setPostProcessShader(std::shared_ptr<Shader> shader)
{
	m_screenShader = shader;
}

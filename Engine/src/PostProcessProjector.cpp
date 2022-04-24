#include "PostProcessProjector.h"

#include "Model.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "Renderer2D.h"
#include "ScreenQuad.h"
#include "Engine.h"
#include "Window.h"
#include "Logger.h"
#include "TextureHandler.h"

bool PostProcessProjector::init()
{
	// Create FBO
	m_frameBuffer = std::make_shared<FrameBufferObject>();
	m_frameBuffer->bind();

	// Get window width and height
	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	// Create a empty texture and attach to FBO
	m_textureHandler = std::shared_ptr<TextureHandler>(Texture::createEmptyTexture(width, height));
	m_frameBuffer->attachTexture(m_textureHandler->getID());

	// Create RBO and attach to FBO
	m_renderBuffer = std::make_shared<RenderBufferObject>();
	m_frameBuffer->attachRenderBuffer(m_renderBuffer->GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	// validate FBO
	if (!m_frameBuffer->isComplete())
	{
		logError("Framebuffer is not complete!");
		return false;
	}
	
	// Cleanup
	m_frameBuffer->unbind();

	// Generate screen quad
	m_quad = ScreenQuad::GenerateScreenQuad();
	
	// Generate screen shader
	m_screenShader = std::make_shared<Shader>("Resources\\Shaders\\PostProcess\\PostProcessShader_default.vert", "Resources\\Shaders\\PostProcess\\PostProcessShader_default.frag");

	// Generate screen renderer
	m_renderer = std::make_shared<Renderer2D>();
}

void PostProcessProjector::enableWriting()
{
	m_frameBuffer->bind();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessProjector::disableWriting()
{
	m_frameBuffer->unbind();
	glDisable(GL_DEPTH_TEST);
}

void PostProcessProjector::draw()
{
	// Clean buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_screenShader->use();

	m_textureHandler->bind();

	m_quad->draw(*m_renderer.get(), m_screenShader.get());

	m_textureHandler->unbind();
}

void PostProcessProjector::setPostProcessShader(std::shared_ptr<Shader> shader)
{
	m_screenShader = shader;
}

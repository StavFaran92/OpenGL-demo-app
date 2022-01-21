#include "ScreenBufferProjector.h"

#include "Graphics/Geometry/Model.h"
#include "Graphics/Buffers/FrameBufferObject.h"
#include "Graphics/Buffers/RenderBufferObject.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Graphics/Models/ScreenQuad.h"
#include "Core/Application.h"
#include "Window.h"

bool ScreenBufferProjector::Init()
{
	m_frameBuffer = std::make_shared<FrameBufferObject>();
	m_frameBuffer->Bind();

	m_texture = Texture::CreateEmptyTexture(Application::Get().GetWindow()->getWidth(), Application::Get().GetWindow()->getHeight());
	m_frameBuffer->AttachTexture(m_texture->GetID());

	m_renderBuffer = std::make_shared<RenderBufferObject>();

	m_frameBuffer->AttachRenderBuffer(m_renderBuffer->GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	if (!m_frameBuffer->IsComplete())
	{
		logError("Framebuffer is not complete!");
		return false;
	}
	m_frameBuffer->Unbind();

	m_quad = ScreenQuad::GenerateScreenQuad();
	m_screenShader = std::make_shared<Shader>("Resources\\Shaders\\SimpleShader.vert", "Resources\\Shaders\\SimpleShader.frag");
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

	m_screenShader->UseShader();
	//glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, m_texture->GetID());
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	m_quad->Draw(m_renderer, m_screenShader);
}

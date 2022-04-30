#include "ObjectPicker.h"

#include "GL/glew.h"

#include "Engine.h"
#include "Context.h"
#include "Scene.h"
#include "Window.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"

#include "Logger.h"

bool ObjectPicker::init(int windowWidth, int windowHeight)
{
	// Create FBO
	m_frameBuffer = std::make_shared<FrameBufferObject>();
	m_frameBuffer->bind();

	// Get window width and height
	m_width = windowWidth;
	m_height = windowHeight;

	// Create a empty texture and attach to FBO
	m_textureHandler = std::shared_ptr<TextureHandler>(Texture::createEmptyTexture(m_width, m_height, GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT));
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

	return true;
}

int ObjectPicker::pickObject(int x, int y)
{
	m_frameBuffer->bind();

	glReadBuffer(GL_COLOR_ATTACHMENT0);

	PixelInfo pixel;
	glReadPixels(x, m_height - y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel);

	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	if (pixel.ObjectID != 0)
	{
		logInfo("Selected Object: " + std::to_string(pixel.ObjectID - 1));

		return pixel.ObjectID - 1;
	}

	return -1;
}

void ObjectPicker::enableWriting()
{
	//m_frameBuffer->bind();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer->getID());

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void ObjectPicker::disableWriting()
{
	//m_frameBuffer->unbind();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}
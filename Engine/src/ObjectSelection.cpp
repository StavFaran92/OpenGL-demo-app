#include "ObjectSelection.h"

#include "GL/glew.h"

#include "Engine.h"
#include "Window.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"

#include "Logger.h"

ObjectSelection::ObjectSelection() :
	m_selectedObject(0)
{
}

ObjectSelection::~ObjectSelection()
{
}

bool ObjectSelection::selectedObject(int x, int y)
{
	m_frameBuffer->bind();

	glReadBuffer(GL_COLOR_ATTACHMENT0);

	PixelInfo pixel;
	glReadPixels(x, Engine::get()->getWindow()->getHeight() - y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel);

	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	if (pixel.ObjectID == 0)
		m_selectedObject = -1;
	else
		m_selectedObject =  pixel.ObjectID - 1;

	logInfo("Selected Object: " + std::to_string(m_selectedObject));

	return true;
}

int ObjectSelection::getSelectedObject() const
{
	return m_selectedObject;
}

bool ObjectSelection::init()
{
	// Create FBO
	m_frameBuffer = std::make_shared<FrameBufferObject>();
	m_frameBuffer->bind();

	// Get window width and height
	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	// Create a empty texture and attach to FBO
	m_textureHandler = std::shared_ptr<TextureHandler>(Texture::createEmptyTexture(width, height, GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT));
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

void ObjectSelection::enableWriting()
{
	//m_frameBuffer->bind();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer->getID());

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void ObjectSelection::disableWriting()
{
	//m_frameBuffer->unbind();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}
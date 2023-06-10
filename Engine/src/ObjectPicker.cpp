#include "ObjectPicker.h"

#include "GL/glew.h"

#include "Engine.h"
#include "Context.h"
#include "Scene.h"
#include "Window.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"
#include "Input.h"
#include "IRenderer.h"
#include "ICamera.h"
#include "PickingShader.h"
#include "Model.h"
#include "Entity.h"

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

ObjectPicker::ObjectPicker(Context* context, Scene* scene)
{
	scene->addRenderCallback(Scene::RenderPhase::PRE_RENDER_BEGIN, [=](const Scene::Params* params) {
		// Picking Phase
		if (scene->isObjectSelectionEnabled() && m_isPickingPhaseActive)
		{
			//// Enable writing to picking frame buffer
			//enableWriting();

			//// Set uniforms in picking shader
			//auto pickingShader = context->getPickingShader();
			//pickingShader->use();
			//pickingShader->setViewMatrix(params->renderer->getCamera()->getView());
			//pickingShader->setProjectionMatrix(params->renderer->getProjection());

			//for (auto [entity, mesh, transform] : params->registry->group<Mesh, Transformation>().each())
			//{
			//	pickingShader->use();
			//	pickingShader->setModelMatrix(transform.getMatrix());
			//	pickingShader->setObjectIndex(model.getID() + 1);
			//	pickingShader->release();

			//	// Draw model
			//	Entity entityhandler{ entity, scene };
			//	params->renderer->render(&entityhandler, &mesh, &transform, pickingShader);
			//}

			//// Release picking shader and stop writing to frame buffer
			//pickingShader->release();
			//disableWriting();

			//// Get mouse X & Y
			//int x, y;
			//Engine::get()->getInput()->getMouse()->getMousePosition(x, y);

			//// Pick object in scene according to X & Y
			//auto objectID = pickObject(x, y);



			//// If object returned != -1 then an object has been picked (-1 means background)
			//if (objectID != -1)
			//{
			//	auto obj = Engine::get()->getObjectManager()->getObjectById(objectID);
			//	if (obj)
			//	{
			//		obj->pick();
			//		obj->select();

			//	}
			//}

			//// Turn picking phase flag off
			//m_isPickingPhaseActive = false;
		}
	});

	Engine::get()->getInput()->getMouse()->onMousePressed(Mouse::MouseButton::LeftMousebutton, [&](SDL_Event e) {
		m_isPickingPhaseActive = true;
	});
}

int ObjectPicker::pickObject(int x, int y)
{
	//m_frameBuffer->bind();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer->getID());

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

	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void ObjectPicker::disableWriting()
{
	//m_frameBuffer->unbind();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

bool ObjectPicker::isPickingPhaseActive() const
{
	return m_isPickingPhaseActive;
}

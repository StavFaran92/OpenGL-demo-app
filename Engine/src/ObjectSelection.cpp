#include "ObjectSelection.h"

#include "GL/glew.h"

#include "Engine.h"
#include "Window.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"
#include "Scene.h"
#include "Context.h"
#include "Shader.h"
#include "Model.h"

#include "Logger.h"

ObjectSelection::ObjectSelection(Context* context, Scene* scene)
	: m_isObjectSelectionEnabled(false)
{
	scene->addRenderCallback(Scene::RenderPhase::PRE_RENDER, [=](const Scene::Params* params) {

		if (isEnabled() && params->scene->isPickingPhaseActive())
		{
			// Clears previous object selection
			clear();
		}
		});

	scene->addRenderCallback(Scene::RenderPhase::DRAW_QUEUE_PRE_RENDER, [=](const Scene::Params* params) {

		auto drawQueueRenderParams = (Scene::DrawQueuePreRenderParams*)params;

		auto phongShader = context->getStandardShader();
		phongShader->use();

		// If model is selected highlight it's color
		// TODO fix
		//if (m_isObjectSelectionEnabled && isObjectSelected(drawQueueRenderParams->model->getID()))
		//	phongShader->setColorMul({ 0.3f, 0.3f, 0.3f, 0.3f });
		//else
		//	phongShader->setColorMul({ 0.f, 0.f, 0.f, 0.f });

		phongShader->release();
		});


}

ObjectSelection::~ObjectSelection()
{
}

void ObjectSelection::enableObjectSelection(bool enable)
{
	m_isObjectSelectionEnabled = enable;
}

bool ObjectSelection::isEnabled() const
{
	return m_isObjectSelectionEnabled;
}

void ObjectSelection::deselectedObject(uint32_t id)
{
	m_selectedObjects.erase(id);
}

void ObjectSelection::clear()
{
	m_selectedObjects.clear();
}

bool ObjectSelection::isObjectSelected(uint32_t id) const
{
	return m_selectedObjects.find(id) != m_selectedObjects.end();
}

bool ObjectSelection::selectObject(uint32_t id)
{
	m_selectedObjects.insert(id);

	return true;
}

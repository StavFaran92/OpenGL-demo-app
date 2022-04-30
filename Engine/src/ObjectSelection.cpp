#include "ObjectSelection.h"

#include "GL/glew.h"

#include "Engine.h"
#include "Window.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"

#include "Logger.h"

ObjectSelection::ObjectSelection()
{
}

ObjectSelection::~ObjectSelection()
{
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

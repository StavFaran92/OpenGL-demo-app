#include "ObjectSelection.h"

#include "Logger.h"

ObjectSelection::ObjectSelection() :
	m_selectedObject(0)
{
}

void ObjectSelection::init()
{
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xff);
	glClearStencil(0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void ObjectSelection::OnMousePressed(SDL_MouseButtonEvent& e)
{
	GLuint index = 0;
	// TODO fix
	glReadPixels(e.x, 768 - e.y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

	SelectObject(index);
}

void ObjectSelection::SelectObject(uint32_t index)
{
	m_selectedObject = index;

	logInfo("Selected object: {}", index);
}

uint32_t ObjectSelection::GetSelectedObject()
{
	return m_selectedObject;
}

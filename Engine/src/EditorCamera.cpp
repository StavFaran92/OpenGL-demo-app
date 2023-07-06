#include "EditorCamera.h"

#include <gl/glew.h>

#include "ApplicationConstants.h"
#include "Engine.h"
#include "Context.h"
#include "ObjectSelection.h"
#include "Scene.h"
#include "EventSystem.h"

#include "Logger.h"

EditorCamera::EditorCamera(Transformation& transform, float startMoveSpeed, float startTurnSpeed)
	: m_transform(transform),
	m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_movementSpeed(startMoveSpeed),
	m_turnSpeed(startTurnSpeed),
	m_distance(5)
{
	calculateOrientation();

	auto eventSystem = Engine::get()->getEventSystem();

	eventSystem->addEventListener(SDL_MOUSEMOTION, [this](SDL_Event e) { 
		OnMouseMotion(e.motion.xrel, e.motion.yrel);
	});
	eventSystem->addEventListener(SDL_MOUSEBUTTONDOWN, [this](SDL_Event e) {
		OnMousePressed(e.button);
	});
	eventSystem->addEventListener(SDL_MOUSEBUTTONUP, [this](SDL_Event e) {
		OnMouseReleased(e.button);
	});
	eventSystem->addEventListener(SDL_MOUSEWHEEL, [this](SDL_Event e) {
		OnMouseScroll(e.wheel.y);
	});

}

void EditorCamera::OnMouseMotion(float xChange, float yChange)
{

	if (!m_isLocked)
	{
		m_angleX += xChange * m_turnSpeed;
		m_angleY += yChange * m_turnSpeed;

		m_angleY = std::clamp(m_angleY, -89.f, 89.f);

		calculateOrientation();
	}
}

glm::mat4 EditorCamera::getView()
{
	
	return glm::lookAt(m_transform.getPosition(), m_center, m_up);
};

void EditorCamera::OnMousePressed(SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_RIGHT)
	{
		m_isLocked = false;
	}
}

void EditorCamera::OnMouseReleased(SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_RIGHT)
	{
		m_isLocked = true;
	}
}

void EditorCamera::OnMouseScroll(Sint32& y)
{
	m_distance = std::clamp(m_distance-y, 1.f, 50.f);

	calculateOrientation();
}

void EditorCamera::lookAt(float x, float y, float z)
{
	m_center = { x, y, z };
}

void EditorCamera::setPosition(float distance, float angleX, float angleY)
{
	m_distance = distance;
	m_angleX = angleX;
	m_angleY = angleY;

	calculateOrientation();
}

void EditorCamera::calculateOrientation()
{
	float t = m_distance * cos(m_angleY * Constants::toRadians);
	float y = m_distance * sin(m_angleY * Constants::toRadians);
	float x = t * cos(m_angleX * Constants::toRadians);
	float z = t * sin(m_angleX * Constants::toRadians);

	m_transform.setPosition(glm::vec3(x, y, z) + m_center);

	m_front = glm::normalize(-m_transform.getPosition() + m_center);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

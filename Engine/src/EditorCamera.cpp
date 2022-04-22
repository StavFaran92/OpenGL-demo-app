#include "EditorCamera.h"

#include <gl/glew.h>

#include "ApplicationConstants.h"
#include "Engine.h"
#include "Context.h"
#include "ObjectSelection.h"
#include "Scene.h"
#include "EventSystem.h"

#include "Logger.h"

EditorCamera::EditorCamera(glm::vec3 startPosition, float startMoveSpeed, float startTurnSpeed)
	: m_position(startPosition),
	m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_movementSpeed(startMoveSpeed),
	m_turnSpeed(startTurnSpeed),
	distance(5)
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

void EditorCamera::keyControl(double deltaTime)
{
	if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_DELETE))
	{
		auto objectSelection = Engine::get()->getContext()->getActiveScene()->GetObjectSelection();
		auto selected = objectSelection->GetSelectedObject();
		Engine::get()->getContext()->getActiveScene()->removeModel(selected);
	}
}

void EditorCamera::OnMouseMotion(float xChange, float yChange)
{

	if (!m_isLocked)
	{
		m_angleX += xChange * m_turnSpeed;
		m_angleY += yChange * m_turnSpeed;

		m_angleY = std::clamp(m_angleY, -90.f, 90.f);

		calculateOrientation();
	}
}

glm::vec3 EditorCamera::getPosition()
{
	return m_position;
}

void EditorCamera::update(float deltaTime)
{
	keyControl(deltaTime);
}

void EditorCamera::OnMousePressed(SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_RIGHT)
	{
		m_isLocked = false;
	}
	if (e.button == SDL_BUTTON_LEFT)
	{

		auto objectSelection = Engine::get()->getContext()->getActiveScene()->GetObjectSelection();
		objectSelection->OnMousePressed(e);

		logInfo("Mouse pressed on x: {}, y: {}", e.x, e.y);
		
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
	distance = std::clamp(distance-y, 1.f, 50.f);

	calculateOrientation();
}

void EditorCamera::calculateOrientation()
{
	float t = distance * cos(m_angleY * Constants::toRadians);
	float y = distance * sin(m_angleY * Constants::toRadians);
	float x = t * cos(m_angleX * Constants::toRadians);
	float z = t * sin(m_angleX * Constants::toRadians);

	m_position = glm::vec3(x, y, z);

	m_front = glm::normalize(-m_position);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

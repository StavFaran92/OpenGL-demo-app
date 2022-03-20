#include "EditorCamera.h"

#include "Engine.h"
#include "Context.h"
#include "ObjectSelection.h"
#include "Scene.h"

EditorCamera::EditorCamera(glm::vec3 startPosition, float startMoveSpeed, float startTurnSpeed)
	: m_position(startPosition),
	m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_movementSpeed(startMoveSpeed),
	m_turnSpeed(startTurnSpeed),
	distance(5)
{
	m_keyboard = std::make_shared<Keyboard>();

	calculateOrientation();
}

void EditorCamera::keyControl(double deltaTime)
{
	if (m_keyboard->getKeyState(SDL_SCANCODE_DELETE))
	{
		auto objectSelection = Engine::get()->getContext()->getActiveScene()->GetObjectSelection();
		auto selected = objectSelection->GetSelectedObject();
		Engine::get()->getContext()->getActiveScene()->removeModel(selected);
	}
	if (m_keyboard->getKeyState(SDL_SCANCODE_2))
	{
		Engine::get()->getContext()->setActiveScene(2);

	}
	if (m_keyboard->getKeyState(SDL_SCANCODE_1))
	{
		Engine::get()->getContext()->setActiveScene(1);

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

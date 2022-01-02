#include "EditorCamera.h"

EditorCamera::EditorCamera(glm::vec3 startPosition, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	: m_position(startPosition),
	m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_movementSpeed(startMoveSpeed),
	m_turnSpeed(startTurnSpeed)
{
	m_keyboard = std::make_shared<Keyboard>();

	m_front = glm::normalize(-m_position);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	distance = glm::length(m_position);
}

void EditorCamera::keyControl(double deltaTime)
{
	//if (m_keyboard->getKeyState(SDL_SCANCODE_W))
	//{
	//	logInfo("mouse button pressed!");
	//	m_isLocked = false;
	//}
	//if (m_keyboard->getKeyState(SDL_SCANCODE_W))
	//{
	//	logInfo("mouse button released!");
	//	m_isLocked = true;
	//}
}

void EditorCamera::OnMouseMotion(GLfloat xChange, GLfloat yChange)
{

	if (!m_isLocked)
	{
		m_angleX += xChange * m_turnSpeed;
		m_angleY += yChange * m_turnSpeed;

		m_angleY = std::clamp(m_angleY, -90.f, 90.f);

		float t = distance * cos(m_angleY * Constants::toRadians);
		float y = distance * sin(m_angleY * Constants::toRadians);
		float x = t * cos(m_angleX * Constants::toRadians);
		float z = t * sin(m_angleX * Constants::toRadians);

		m_position = glm::vec3(x, y, z);

		m_front = glm::normalize(-m_position);

		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}
}

glm::vec3 EditorCamera::getPosition()
{
	return m_position;
}

void EditorCamera::update(float deltaTime)
{
}

void EditorCamera::OnMousePressed(SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		m_isLocked = false;
	}
}

void EditorCamera::OnMouseReleased(SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		m_isLocked = true;
	}
}

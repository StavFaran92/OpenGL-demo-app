#include "CameraControllerOrbit.h"

#include "Engine.h"
#include "EventSystem.h"
#include "ApplicationConstants.h"
#include <algorithm>
#include "glm/glm.hpp"

void CameraControllerOrbit::calculateOrientation()
{
	float t = m_distance * cos(m_angleY * Constants::toRadians);
	float y = m_distance * sin(m_angleY * Constants::toRadians);
	float x = t * cos(m_angleX * Constants::toRadians);
	float z = t * sin(m_angleX * Constants::toRadians);

	m_cameraComponent->position = glm::vec3(x, y, z) + m_cameraComponent->center;

	auto front = glm::normalize(-m_cameraComponent->position + m_cameraComponent->center);
	auto right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_cameraComponent->up = glm::normalize(glm::cross(right, front));
}

void CameraControllerOrbit::onCreate(CameraComponent* cameraComponent)
{
	m_cameraComponent = cameraComponent;

	auto eventSystem = Engine::get()->getEventSystem();

	eventSystem->addEventListener(SDL_MOUSEMOTION, [this](SDL_Event e)
		{
			int xChange = e.motion.xrel;
			int yChange = e.motion.yrel;
			if (!m_isLocked)
			{
				m_angleX += xChange * m_turnSpeed;
				m_angleY += yChange * m_turnSpeed;

				m_angleY = std::clamp(m_angleY, -89.f, 89.f);

				calculateOrientation();
			}
		});
	eventSystem->addEventListener(SDL_MOUSEBUTTONDOWN, [this](SDL_Event e)
		{
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				m_isLocked = false;
			}
		});
	eventSystem->addEventListener(SDL_MOUSEBUTTONUP, [this](SDL_Event e)
		{
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				m_isLocked = true;
			}
		});
	eventSystem->addEventListener(SDL_MOUSEWHEEL, [this](SDL_Event e)
		{
			m_distance = std::clamp(m_distance - e.wheel.y, 1.f, 50.f);

			calculateOrientation();
		});
}

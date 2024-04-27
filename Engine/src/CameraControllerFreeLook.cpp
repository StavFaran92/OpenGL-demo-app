#include "CameraControllerFreeLook.h"

#include "Engine.h"
#include "EventSystem.h"
#include "ApplicationConstants.h"
#include <algorithm>
#include "glm/glm.hpp"

void CameraControllerFreeLook::calculateOrientation()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(m_front);
	m_cameraComponent->center = m_cameraComponent->getPosition() + m_front;

	m_right = glm::normalize(glm::cross(m_front, {0,1,0}));
	m_cameraComponent->up = glm::normalize(glm::cross(m_right, m_front));
}

void CameraControllerFreeLook::onCreate(CameraComponent* cameraComponent)
{
	m_cameraComponent = cameraComponent;

	m_cameraComponent->position = { 0, 10, 0 };

	auto eventSystem = Engine::get()->getEventSystem();

	eventSystem->addEventListener(SDL_MOUSEMOTION, [this](SDL_Event e)
		{
			
			if (!m_isLocked)
			{
				int xChange = e.motion.xrel;
				int yChange = e.motion.yrel;

				xChange *= m_turnSpeed;
				yChange *= m_turnSpeed;

				m_yaw += xChange;
				m_pitch -= yChange;

				if (m_pitch > 89.0f)
				{
					m_pitch = 89.0f;
				}

				if (m_pitch < -89.0f)
				{
					m_pitch = -89.0f;
				}

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

#pragma once

#include "sge.h"

class CameraScript : public ScriptableEntity
{
	virtual void onCreate() override
	{
		Engine::get()->getWindow()->lockMouse();

		auto eventSystem = Engine::get()->getEventSystem();

		m_camera = entity;

		m_handler = eventSystem->addEventListener(SDL_MOUSEMOTION, [this](SDL_Event e) {
			OnMouseMotion(e.motion.xrel, e.motion.yrel);
			});
	}

	virtual void onDestroy() override
	{
		Engine::get()->getWindow()->unlockMouse();

		auto eventSystem = Engine::get()->getEventSystem();

		eventSystem->removeEventListener(m_handler);
	}

	virtual void onUpdate(float deltaTime) {
	};

	void OnMouseMotion(float xChange, float yChange)
	{
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

	void calculateOrientation()
	{
		m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front.y = sin(glm::radians(m_pitch));
		m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		m_front = glm::normalize(m_front);

		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));

		auto& camComponent = m_camera.getComponent<CameraComponent>();
		auto& camTransform = m_camera.getComponent<Transformation>();



		// Create quaternions for pitch and yaw
		glm::quat pitchQuat = glm::angleAxis(Constants::PI - m_pitch * Constants::toRadians, glm::vec3(1, 0, 0));
		glm::quat yawQuat = glm::angleAxis(Constants::PI - m_yaw * Constants::toRadians, glm::vec3(0, 1, 0));

		// Combine the quaternions
		glm::quat combinedQuat = yawQuat * pitchQuat;

		camTransform.setLocalRotation(combinedQuat);

		camComponent.up = m_up;
		camComponent.center = camTransform.getWorldPosition() + m_front;
		camComponent.front = m_front;
		camComponent.right = m_right;
	}

	uint64_t m_handler;

	glm::vec3 m_front{ 0.0f, 0.0f, -1.f };
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp{0.0f, 1.0f, 0.0f};

	float m_yaw = 0;
	float m_pitch = 0;

	float m_movementSpeed = 3;
	float m_turnSpeed = 1;

	Entity m_camera;
};
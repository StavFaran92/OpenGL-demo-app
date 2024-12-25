#pragma once

#include <glm/glm.hpp>

#include "ICameraController.h"
#include "Component.h"

class CameraControllerFreeLook : public ICameraController
{
public:
	void onCreate(Entity& e) override;

	void calculateOrientation();
private:
	enum class ControllerState
	{
		IDLE,
		ROTATE,
		TRANSFORM
	};
private:
	bool m_isLocked = true;

	//float m_yaw = 0;
	//float m_pitch = 0;
	glm::vec3 m_up = { 0,1,0 };
	glm::vec3 m_right;
	glm::vec3 m_front;

	float m_turnSpeed = 10.f;
	float m_distance = 0;
	float m_movementSpeed = 10.f;

	ControllerState m_state = ControllerState::IDLE;

	CameraComponent* m_cameraComponent = nullptr;
	Transformation* m_cameraTransform = nullptr;
};
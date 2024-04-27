#pragma once

#include <glm/glm.hpp>

#include "ICameraController.h"
#include "Component.h"

class CameraControllerFreeLook : public ICameraController
{
public:
	void onCreate(CameraComponent* cameraComponent) override;

	void calculateOrientation();
private:
	bool m_isLocked = true;

	float m_yaw = 0;
	float m_pitch = 0;
	glm::vec3 m_up = { 0,1,0 };

	float m_turnSpeed = 0.5f;
	float m_distance = 0;

	CameraComponent* m_cameraComponent = nullptr;
};
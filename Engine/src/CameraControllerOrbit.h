#pragma once

#include <glm/glm.hpp>

#include "ICameraController.h"
#include "Component.h"

class CameraControllerOrbit : public ICameraController
{
public:
	void onCreate(CameraComponent* cameraComponent) override;

	void calculateOrientation();
private:
	bool m_isLocked = true;

	float m_distance = 0;
	float m_angleX = 0;
	float m_angleY = 0;

	float m_turnSpeed = 0.5f;

	CameraComponent* m_cameraComponent = nullptr;
};
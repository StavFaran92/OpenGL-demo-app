#pragma once

#include "ICamera.h"
#include "Core.h"
#include "Component.h"
#include "ScriptableEntity.h"


class EngineAPI EditorCamera : public ICamera
{
public:
	EditorCamera();
	EditorCamera(float startMoveSpeed, float startTurnSpeed);

	void lookAt(float x, float y, float z) override;
	void setPosition(float distance, float angleX, float angleY) override;
	glm::mat4 getView() override;
	glm::vec3 getPosition() override;

	void onCreate() override;
private:
	void calculateOrientation();
private:
	bool m_isLocked = true;

	float m_movementSpeed = 0;
	float m_turnSpeed = 0;
	float m_distance = 0;
	float m_angleX = 0;
	float m_angleY = 0;

	CameraComponent* m_cameraComponent = nullptr;

};


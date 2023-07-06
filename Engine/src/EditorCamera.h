#pragma once

#include "ICamera.h"
#include "Core.h"
#include "Component.h"
#include "ScriptableEntity.h"


class EngineAPI EditorCamera : public ScriptableEntity
{
public:
	EditorCamera();
	EditorCamera(float startMoveSpeed, float startTurnSpeed);

	void OnMouseMotion(float xChange, float yChange);
	void OnMousePressed(SDL_MouseButtonEvent& e);
	void OnMouseReleased(SDL_MouseButtonEvent& e);
	void OnMouseScroll(Sint32& y);

	void lookAt(float x, float y, float z);
	void setPosition(float distance, float angleX, float angleY);

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


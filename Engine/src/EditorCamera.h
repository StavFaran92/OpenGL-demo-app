#pragma once

#include "ICamera.h"
#include "Core.h"
#include "Transformation.h"


class EngineAPI EditorCamera : public ICamera
{
public:
	EditorCamera(Transformation& transform, float startMoveSpeed, float startTurnSpeed);
	
	glm::mat4 getView() override;

	void OnMouseMotion(float xChange, float yChange);
	void OnMousePressed(SDL_MouseButtonEvent& e);
	void OnMouseReleased(SDL_MouseButtonEvent& e);
	void OnMouseScroll(Sint32& y);

	void lookAt(float x, float y, float z);
	void setPosition(float distance, float angleX, float angleY);
private:
	void calculateOrientation();
private:
	bool m_isLocked = true;

	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	glm::vec3 m_center;

	float m_movementSpeed = 0;
	float m_turnSpeed = 0;
	float m_distance = 0;
	float m_angleX = 0;
	float m_angleY = 0;

	Transformation& m_transform;

};


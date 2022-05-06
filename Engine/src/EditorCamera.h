#pragma once

#include "ICamera.h"
#include "Core.h"


class EngineAPI EditorCamera : public ICamera
{
public:
	EditorCamera(glm::vec3 startPosition, float startMoveSpeed, float startTurnSpeed);
	void keyControl(double deltaTime) override;
	void OnMouseMotion(float xChange, float yChange) override;
	glm::mat4 getView() override;
	glm::vec3 getPosition() override;
	void update(float deltaTime) override;
	void OnMousePressed(SDL_MouseButtonEvent& e) override;
	void OnMouseReleased(SDL_MouseButtonEvent& e) override;
	void OnMouseScroll(Sint32& y) override;
	void lookAt(float x, float y, float z);
	void setPosition(float distance, float angleX, float angleY);
private:
	void calculateOrientation();
private:
	bool m_isLocked = true;

	glm::vec3 m_position;
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

};


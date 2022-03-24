#pragma once

#include "ICamera.h"
#include "Core.h"


class EngineAPI EditorCamera : public ICamera
{
public:
	EditorCamera(glm::vec3 startPosition, float startMoveSpeed, float startTurnSpeed);
	void keyControl(double deltaTime) override;
	void OnMouseMotion(float xChange, float yChange) override;
	glm::mat4 getView() override { return glm::lookAt(m_position, {0,0,0}, m_up); };
	glm::vec3 getPosition() override;
	void update(float deltaTime) override;
	void OnMousePressed(SDL_MouseButtonEvent& e) override;
	void OnMouseReleased(SDL_MouseButtonEvent& e) override;
	void OnMouseScroll(Sint32& y) override;
private:
	void calculateOrientation();
private:
	bool m_isLocked = true;

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_movementSpeed = 0;
	float m_turnSpeed = 0;
	float distance = 0;
	float m_angleX = 0;
	float m_angleY = 0;

};


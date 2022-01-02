#pragma once

#include "ICamera.h"
#include "ApplicationConstants.h"

class EditorCamera : public ICamera
{
public:
	EditorCamera(glm::vec3 startPosition, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	void keyControl(double deltaTime) override;
	void OnMouseMotion(GLfloat xChange, GLfloat yChange) override;
	glm::mat4 getView() override { return glm::lookAt(m_position, {0,0,0}, m_up); };
	glm::vec3 getPosition() override;
	void update(float deltaTime) override;
	void OnMousePressed(SDL_MouseButtonEvent& e) override;
	void OnMouseReleased(SDL_MouseButtonEvent& e) override;
private:
	bool m_isLocked = true;
	std::shared_ptr<Keyboard> m_keyboard;

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	GLfloat m_movementSpeed;
	GLfloat m_turnSpeed;
	GLfloat distance;
	GLfloat m_angleX = 0;
	float m_angleY = 0;

};


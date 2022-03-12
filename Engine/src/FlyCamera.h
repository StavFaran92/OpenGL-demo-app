#pragma once

#include "ICamera.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Keyboard.h"

class FlyCamera : public ICamera
{
public:
	FlyCamera(glm::vec3 startPosition, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(double deltaTime) override;
	void OnMouseMotion(GLfloat xChange, GLfloat yChange) override;
	void OnMousePressed(SDL_MouseButtonEvent& e) override;
	void OnMouseReleased(SDL_MouseButtonEvent& e) override;

	inline glm::mat4 getView() override { return glm::lookAt(m_position, m_position + m_front, m_up); }
	inline glm::vec3 getPosition() override { return m_position; }

	void update(float deltaTime) override;

	~FlyCamera();
private:
	void calculateOrientation();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	GLfloat m_yaw;
	GLfloat m_pitch;

	GLfloat m_movementSpeed;
	GLfloat m_turnSpeed;

	std::shared_ptr<Keyboard> keyboard;
};

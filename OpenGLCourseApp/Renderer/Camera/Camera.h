#pragma once

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "IO/Keyboard.h"

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(double deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix();

	glm::vec3 getCameraPosition() { return m_position; }

	void moveRight(float x);

	~Camera();
private:
	void recalculate();
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

	bool m_isCameraLocked;

	std::shared_ptr<Keyboard> keyboard;

};


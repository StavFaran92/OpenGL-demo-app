#pragma once

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "IO/Keyboard.h"

class Camera
{
public:
	Camera(glm::vec3 startPosition, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(double deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	inline glm::mat4 getView() { return glm::lookAt(m_position, m_position + m_front, m_up); }
	inline glm::vec3 getPosition() { return m_position; }

	void update(float deltaTime);

	~Camera();
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

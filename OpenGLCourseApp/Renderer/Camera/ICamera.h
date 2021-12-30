#pragma once

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "IO/Keyboard.h"

class ICamera
{
public:
	ICamera() = default;

	virtual void keyControl(double deltaTime) = 0;
	virtual void mouseControl(GLfloat xChange, GLfloat yChange) = 0;

	virtual glm::mat4 getView() = 0;
	virtual glm::vec3 getPosition() = 0;

	virtual void update(float deltaTime) = 0;

	virtual ~ICamera() = default;
};

#pragma once

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Keyboard.h"

class ICamera
{
public:
	ICamera() = default;

	virtual void keyControl(double deltaTime) = 0;
	virtual void OnMouseMotion(float xChange, float yChange) = 0;
	virtual void OnMousePressed(SDL_MouseButtonEvent& e) = 0;
	virtual void OnMouseReleased(SDL_MouseButtonEvent& e) = 0;
	virtual void OnMouseScroll(Sint32& y) {};

	virtual glm::mat4 getView() = 0;
	virtual glm::vec3 getPosition() = 0;

	virtual void update(float deltaTime) = 0;

	virtual ~ICamera() = default;
};

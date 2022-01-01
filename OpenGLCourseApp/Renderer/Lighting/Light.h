#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "../Shader/Shader.h"

#include "Core/Application.h"
#include "ApplicationConstants.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

	virtual void useLight(std::shared_ptr<Shader> shader);

	virtual ~Light();

protected:
	glm::vec3 m_color;
	GLfloat m_ambientIntensity;
	GLfloat m_diffuseIntensity;

	glm::vec3 m_position;
};

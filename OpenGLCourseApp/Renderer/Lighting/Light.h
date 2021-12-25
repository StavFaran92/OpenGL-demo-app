#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "../Shader/Shader.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

	void useLight(Shader& shader);

	~Light();

protected:
	glm::vec3 m_color;
	GLfloat m_ambientIntensity;
	GLfloat m_diffuseIntensity;
};

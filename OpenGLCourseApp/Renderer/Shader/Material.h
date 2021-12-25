#pragma once

#include <GL/glew.h>

#include "../Shader/Shader.h"

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(Shader& shader);

	~Material();

private:
	GLfloat m_specularIntensity;
	GLfloat m_shininess;
};

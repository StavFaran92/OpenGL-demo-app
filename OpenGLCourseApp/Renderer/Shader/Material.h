#pragma once

#include <GL/glew.h>

#include "Renderer/Shader/Shader.h"

class Material
{
public:
	Material();
	Material(GLfloat shine);

	void UseMaterial(std::shared_ptr<Shader> shader);

	~Material();

private:
	GLfloat m_specularIntensity;
	GLfloat m_shininess;
};

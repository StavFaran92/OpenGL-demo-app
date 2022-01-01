#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight()
		: Light(), m_direction(0.0f, -1.0f, 0.0f)
	{}

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir)
		: Light(red, green, blue, aIntensity, dIntensity), m_direction(xDir, yDir, zDir)
	{}

	void useLight(std :: shared_ptr<Shader> shader) override;

private:
	glm::vec3 m_direction;
};

#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight() 
		: Light(), direction(0.0f, -1.0f, 0.0f) 
	{}

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir)
		: Light(red, green, blue, aIntensity, dIntensity), direction(xDir, yDir, zDir)
	{}

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

private:
	glm::vec3 direction;
};


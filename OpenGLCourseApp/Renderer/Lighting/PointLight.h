#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	//Default constructor
	PointLight() :
		Light(), position(0.0f, 0.0f, 0.0f), constant(1), linear(0), exponent(0)
	{}

	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent) :
		Light(red, green, blue, aIntensity, dIntensity),
		position(xPos, yPos, zPos), constant(constant), linear(linear), exponent(exponent)
	{}

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation,
		GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

private:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};


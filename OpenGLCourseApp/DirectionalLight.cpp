#include "DirectionalLight.h"

void DirectionalLight::useLight(GLfloat ambientIntensityLocation, 
	GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	Light::useLight(ambientIntensityLocation, ambientColorLocation, diffuseIntensityLocation);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}


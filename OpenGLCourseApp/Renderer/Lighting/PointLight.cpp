#include "PointLight.h"

void PointLight::useLight(Shader& shader)
{
	Light::useLight(shader);


	//glUniform3f(positionLocation, position.x, position.y, position.z);
	//glUniform1f(constantLocation, constant);
	//glUniform1f(linearLocation, linear);
	//glUniform1f(exponentLocation, exponent);
}
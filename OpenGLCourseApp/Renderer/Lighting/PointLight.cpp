#include "PointLight.h"

void PointLight::useLight(std::shared_ptr<Shader >shader)
{
	Light::useLight(shader);

	//glUniform3f(positionLocation, position.x, position.y, position.z);
	//glUniform1f(constantLocation, constant);
	//glUniform1f(linearLocation, linear);
	//glUniform1f(exponentLocation, exponent);
}
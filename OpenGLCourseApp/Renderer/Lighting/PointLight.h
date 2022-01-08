#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() :
		Light(), constant(1), linear(1), quadratic(1)
	{
		Init();
	}

	PointLight(glm::vec3 color, glm::vec3 pos, 
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat constant, GLfloat linear, GLfloat quadratic) :
		Light(color, aIntensity, dIntensity),
		constant(constant), linear(linear), quadratic(quadratic)
	{
		Init();

		m_transform->SetPosition(pos);
	}

	void Init();

	void useLight(std::shared_ptr<Shader >shader);

private:
	GLfloat constant, linear, quadratic;

public:
	static uint32_t m_count;
};

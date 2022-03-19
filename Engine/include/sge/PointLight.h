#pragma once
#include "Light.h"

struct Attenuation {
	float constant = 1;
	float linear = .35f;
	float quadratic = .44;
};

class PointLight : public Light
{
public:
	PointLight() :
		Light(), m_attenuation()
	{
		m_name = "pointLights";
	}

	PointLight(glm::vec3 color, glm::vec3 pos, 
		GLfloat aIntensity, GLfloat dIntensity,
		Attenuation attenuation) :
		Light(color, aIntensity, dIntensity),
		m_attenuation(attenuation)
	{
		m_name = "pointLights";

		m_transform->SetPosition(pos);
	}

	void useLight(std::shared_ptr<Shader >shader, int index);

	void SetAttenuation(Attenuation attenuation)
	{
		m_attenuation = attenuation;
	}

private:
	Attenuation m_attenuation;
};

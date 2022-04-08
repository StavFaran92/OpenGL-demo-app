#pragma once
#include "Light.h"
#include "Core.h"


class EngineAPI DirectionalLight : public Light
{
public:
	DirectionalLight()
		: Light(), m_direction(0.0f, -1.0f, 0.0f)
	{
		m_name = "dirLight";
	}

	DirectionalLight(glm::vec3 color, glm::vec3 dir, float aIntensity, float dIntensity)
		: Light(color, aIntensity, dIntensity), m_direction(dir)
	{
		m_name = "dirLight";
	}

	void useLight(Shader& shader, int index) override;

private:
	glm::vec3 m_direction;
};

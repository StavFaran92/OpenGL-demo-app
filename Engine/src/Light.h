#pragma once
#include <glm/glm.hpp>

#include <string>
#include "Core.h"

#include "Object3D.h"

class Shader;

class EngineAPI Light : public Object3D
{
public:
	Light();
	Light(glm::vec3 color, float aIntensity, float dIntensity);

	virtual void useLight(std::shared_ptr<Shader> shader, int index);

	inline void SetAmbientIntensity(float intensity) {
		m_ambientIntensity = intensity;
	}
	inline void SetDiffuseIntensity(float intensity) {
		m_diffuseIntensity = intensity;
	}
	inline void SetColor(glm::vec3 color) {
		m_color = color;
	}

	virtual ~Light();

protected:
	glm::vec3 m_color;
	float m_ambientIntensity;
	float m_diffuseIntensity;

	std::shared_ptr<Transform >m_transform;
	std::string m_name = "";
};

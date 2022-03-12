#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "../Shader/Shader.h"

#include "Core/Engine.h"
#include "ApplicationConstants.h"

#include "Graphics/Geometry/Transform.h"

using namespace Constants;

class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat aIntensity, GLfloat dIntensity);

	virtual void useLight(std::shared_ptr<Shader> shader, int index);

	inline void SetAmbientIntensity(GLfloat intensity) {
		m_ambientIntensity = intensity;
	}
	inline void SetDiffuseIntensity(GLfloat intensity) {
		m_diffuseIntensity = intensity;
	}
	inline void SetColor(glm::vec3 color) {
		m_color = color;
	}

	inline std::shared_ptr<Transform > GetTransform() { return m_transform; }

	virtual ~Light();

protected:
	glm::vec3 m_color;
	GLfloat m_ambientIntensity;
	GLfloat m_diffuseIntensity;

	std::shared_ptr<Transform >m_transform;
	std::string m_name = "";
};

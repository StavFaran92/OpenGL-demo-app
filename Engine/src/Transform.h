#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core.h"

class EngineAPI Transform
{
public:
	Transform() :
		m_translation(0, 0, 0),
		m_orientation(1,0,0,0),
		m_scale(1, 1, 1),
		m_transformation(1.f)
	{}

	inline const glm::mat4 GetTransformation() const { return m_transformation; }
	void update(float deltaTime);
	void SetPosition(glm::vec3 pos);
	void SetRotation(float angle, glm::vec3 axis);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetPosition() const { return m_translation; }
	glm::quat getOrientation() const;
	glm::vec3 GetScale() const { return m_scale; }

	void translate(float x, float y, float z);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);


private:
	glm::vec3 m_translation;

	glm::quat m_orientation;

	glm::vec3 m_scale;

	glm::mat4 m_transformation;


	bool m_change = false;
};

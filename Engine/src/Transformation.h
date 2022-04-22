#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core.h"

class EngineAPI Transformation
{
public:
	Transformation() :
		m_translation(0, 0, 0),
		m_orientationLocal(1, 0, 0, 0),
		m_orientationWorld(1, 0, 0, 0),
		m_scale(1, 1, 1),
		m_transformation(1.f),
		m_relativeRot(1.f)
	{}

	void update(float deltaTime);

	void SetPosition(glm::vec3 pos);
	void SetRotation(float angle, glm::vec3 axis);
	void SetScale(glm::vec3 scale);

	glm::mat4 getTransformation() const;
	glm::vec3 getPosition() const;
	glm::quat getLocalOrientation() const;
	glm::quat getWorldOrientation() const;
	glm::vec3 getScale() const;

	void translate(float x, float y, float z);

	void rotate(glm::vec3 eulers);
	void rotate(glm::vec3 axis, float angle);
	void rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle);
	void rotateLerp(glm::vec3 axis, float angle, float t);
	void rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t);


private:
	glm::vec3 m_translation;

	glm::quat m_orientationLocal;
	glm::quat m_orientationWorld;

	glm::mat4 m_relativeRot;

	glm::vec3 m_scale;

	glm::mat4 m_transformation;

	bool m_change = false;
};

#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core.h"

#include "Entity.h"

class EngineAPI Transformation
{
public:
	Transformation(const Entity& entity) :
		m_entity(entity),
		m_translation(0, 0, 0),
		m_orientation(1, 0, 0, 0),
		m_scale(1, 1, 1),
		m_transformation(1.f),
		m_relativeRot(1.f)
	{}

	Transformation(const Entity& entity, glm::vec3 translation) :
		m_entity(entity),
		m_translation(translation),
		m_orientation(1, 0, 0, 0),
		m_scale(1, 1, 1),
		m_transformation(1.f),
		m_relativeRot(1.f)
	{
		m_change = true;
		update(0);
	}

	Transformation(const Entity& entity, glm::vec3 translation, glm::quat rotation) :
		m_entity(entity),
		m_translation(translation),
		m_orientation(rotation),
		m_scale(1, 1, 1),
		m_transformation(1.f),
		m_relativeRot(1.f)
	{
		m_change = true;
		update(0);
	}

	void update(float deltaTime);

	void setPosition(glm::vec3 pos);
	void setRotation(float angle, glm::vec3 axis);
	void setRotation(glm::quat quat);
	void setScale(glm::vec3 scale);

	glm::mat4 getMatrix();
	glm::vec3 getPosition() const;
	glm::quat getOrientation() const;
	glm::vec3 getScale() const;

	void translate(float x, float y, float z);
	void translate(glm::vec3 translation);

	void scale(float x, float y, float z);
	void scale(glm::vec3 translation);

	void rotate(glm::vec3 eulers);
	void rotate(glm::vec3 axis, float angle);
	void rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle);
	void rotateLerp(glm::vec3 axis, float angle, float t);
	void rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t);
	void getMatrix(glm::mat4& mat);

	void setParent(Entity parent);
	Entity getParent() const;
	void addChild(Entity entity);
	void removeChild(Entity entity);
	std::unordered_map<entity_id, Entity> getChildren();

private:
	glm::vec3 m_translation;

	glm::quat m_orientation;

	glm::mat4 m_relativeRot;

	glm::vec3 m_scale;

	glm::mat4 m_transformation;

	Entity m_parent;
	Entity m_entity;
	std::unordered_map<entity_id, Entity> m_children{};

	bool m_change = false;
};

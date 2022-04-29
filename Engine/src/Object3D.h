#pragma once

#include <memory>
#include <vector>
#include "Core.h"
#include "glm/glm.hpp"


class Transformation;
class Scene;

class EngineAPI Object3D
{
public:
	Object3D();
	virtual ~Object3D() = default;

	Transformation* getTransformation() const;
	uint32_t getID() const;

	void translate(float x, float y, float z) const;
	void translate(glm::vec3 translation) const;
	void scale(float x, float y, float z) const;
	void scale(glm::vec3 scaleFactor) const;
	void rotate(glm::vec3 eulers) const;
	void rotate(glm::vec3 axis, float angle) const;
	void rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle) const;
	void rotateLerp(glm::vec3 axis, float angle, float t) const;
	void rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t) const;

	void addChildren(std::shared_ptr<Object3D> child);
	void addChildren(Object3D* child);

	void removeChildren(std::shared_ptr<Object3D> child);
	void removeChildren(Object3D* child);

protected:
	friend class Scene;
	void setSceneID(uint32_t id);
	virtual void update(float deltatime);

protected:
	uint32_t m_sceneID = 0;
	uint32_t m_id = 0;

	std::shared_ptr<Transformation> m_transformation;

	std::vector<Object3D*> m_childrens;

	static uint32_t s_modelCounter;
};

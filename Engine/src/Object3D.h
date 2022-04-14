#pragma once

#include <memory>
#include "Core.h"
#include "glm/glm.hpp"

class Transformation;
class Scene;

class EngineAPI Object3D
{
public:
	Object3D() = default;
	virtual ~Object3D() = default;

	Transformation* getTransformation() const;
	uint32_t getID() const;

	void translate(float x, float y, float z) const;
	void rotate(glm::vec3 eulers) const;
	void rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle) const;

protected:
	friend class Scene;
	inline void setID(uint32_t id) { m_id = id; }

protected:
	uint32_t m_id = 0;

	std::shared_ptr<Transformation> m_transformation;
};

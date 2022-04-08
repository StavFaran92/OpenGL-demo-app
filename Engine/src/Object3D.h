#pragma once

#include <memory>
#include "Core.h"

class Transform;
class Scene;

class EngineAPI Object3D
{
public:
	Object3D() = default;
	virtual ~Object3D() = default;

	Transform& GetTransformation() const;
	uint32_t getID() const;

	void translate(float x, float y, float z);
	void rotateX(float x);
	void rotateY(float y);
	void rotateZ(float z);
protected:
	friend class Scene;
	inline void setID(uint32_t id) { m_id = id; }

protected:
	uint32_t m_id = 0;

	std::shared_ptr<Transform> m_transformation;
};

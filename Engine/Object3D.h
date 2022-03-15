#pragma once

#include <memory>

class Transform;
class Scene;

class Object3D
{
public:
	Object3D() = default;
	virtual ~Object3D() = default;

	std::shared_ptr<Transform> GetTransformation() const;
	uint32_t getID() const;
private:
	friend class Scene;
	inline void setID(uint32_t id) { m_id = id; }

protected:
	uint32_t m_id = 0;

	std::shared_ptr<Transform> m_transformation;
};

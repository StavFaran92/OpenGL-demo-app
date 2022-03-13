#pragma once

#include <memory>

class Transform;

class Object3D
{
public:
	std::shared_ptr<Transform> GetTransformation() const;
	inline uint32_t getID() const;
private:
	friend class Context;
	inline void SetID(uint32_t id) { m_id = id; }

private:
	uint32_t m_id = 0;

	std::shared_ptr<Transform> m_transformation;
};

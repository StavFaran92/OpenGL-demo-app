#pragma once

#include <glm/glm.hpp>

struct Plane
{
public:
	Plane(glm::vec3 normal, float distance)
		: m_normal(normal), m_distance(distance)
	{};

	glm::vec3 m_normal;

	float m_distance;
};
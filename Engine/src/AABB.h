#pragma once

#include "Volume.h"

struct AABB : public Volume
{
	AABB(glm::vec3 center, glm::vec3 extents) : center(center), extents(extents) {};

	glm::vec3 center;
	glm::vec3 extents;
};
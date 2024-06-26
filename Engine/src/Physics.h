#pragma once

#include "Core.h"

#include <glm/glm.hpp>

class EngineAPI Physics
{
public:
	static bool raycast(glm::vec3 origin, glm::vec3 dir, int maxDistance);
};
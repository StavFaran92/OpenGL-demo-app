#pragma once

#include "Engine.h"
#include "Core.h"

#include <glm/glm.hpp>

class EngineAPI DebugHelper
{
public:
	static void drawPoint(const glm::vec3& pos);

	static void drawLine(const glm::vec3& p1, const glm::vec3& p2);

	static void drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
};
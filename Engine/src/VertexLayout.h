#pragma once

#include <vector>

#include "Core.h"
#include "Configurations.h"


struct EngineAPI VertexLayout
{
	std::vector<LayoutAttribute> attribs;
	size_t numOfVertices = 0;
	size_t stride = 0;
};

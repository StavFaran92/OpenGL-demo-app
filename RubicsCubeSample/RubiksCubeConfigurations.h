#pragma once

#include <string>

namespace rubiksCube
{
	const std::string g_padding = "\t";
	const std::string g_rubiksCubeTexture = "Resources/Content/Textures/plane.png";

	enum class Shift
	{
		CW,
		CCW
	};

	enum class Axis
	{
		X, Y, Z
	};
}
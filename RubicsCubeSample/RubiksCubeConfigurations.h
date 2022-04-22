#pragma once

#include <string>

namespace rubiksCube
{
	const std::string g_padding = "\t";

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
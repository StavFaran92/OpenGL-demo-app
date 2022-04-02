#pragma once

#include <map>

constexpr bool FLIP_TEXTURE = true;

enum class LayoutAttributes
{
	Positions,
	Normals,
	Texcoords,
	Colors
};

const std::map<LayoutAttributes, size_t> g_attributeToSizeMap =
{
	{ LayoutAttributes::Positions, 3},
	{ LayoutAttributes::Normals, 3 },
	{ LayoutAttributes::Texcoords, 2 },
	{ LayoutAttributes::Colors, 3 },
};

size_t getAttributeSize(LayoutAttributes attribute);

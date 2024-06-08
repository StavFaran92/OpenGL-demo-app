#pragma once

#include <map>
#include <vector>

constexpr bool FLIP_TEXTURE = true;
constexpr bool DEBUG_MODE_ENABLED = false;
constexpr bool DEBUG_DISPLAY_NORMALS = false;
static bool SGE_EXPORT_PACKAGE = false;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

enum class LayoutAttribute// : int
{
	Positions,
	Normals,
	Texcoords,
	Colors,
	Tangents,

	InstanceModel_0,
	InstanceModel_1,
	InstanceModel_2,
	InstanceModel_3,
};

const std::map<LayoutAttribute, size_t> g_attributeToSizeMap =
{
	{ LayoutAttribute::Positions, 3},
	{ LayoutAttribute::Normals, 3 },
	{ LayoutAttribute::Texcoords, 2 },
	{ LayoutAttribute::Colors, 3 },
	{ LayoutAttribute::Tangents, 3 },

	{ LayoutAttribute::InstanceModel_0, 4 },
	{ LayoutAttribute::InstanceModel_1, 4 },
	{ LayoutAttribute::InstanceModel_2, 4 },
	{ LayoutAttribute::InstanceModel_3, 4 },
};

size_t getAttributeSize(LayoutAttribute attribute);

const std::map<LayoutAttribute, size_t> g_attributeToLocationMap =
{
	{ LayoutAttribute::Positions, 0},
	{ LayoutAttribute::Normals, 1 },
	{ LayoutAttribute::Texcoords, 2 },
	{ LayoutAttribute::Colors, 3 },
	{ LayoutAttribute::Tangents, 4 },

	{ LayoutAttribute::InstanceModel_0, 6 },
	{ LayoutAttribute::InstanceModel_1, 7 },
	{ LayoutAttribute::InstanceModel_2, 8 },
	{ LayoutAttribute::InstanceModel_3, 9 },
};

size_t getAttributeLocationInShader(LayoutAttribute attribute);

const std::vector<LayoutAttribute> g_defaultLayoutAttributes =
{
	LayoutAttribute::Positions,
	LayoutAttribute::Normals,
	LayoutAttribute::Texcoords,
};

enum class RigidbodyType : int
{
	Static = 0,
	Dynamic,
	Kinematic
};
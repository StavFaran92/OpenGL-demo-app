#pragma once

#include <map>
#include <vector>
#include <string>
#include <typeinfo>

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

struct AttributeData
{
	size_t length = 0;
	size_t size = 0;
	int location = 0;
	std::string typeName;
};

const std::map<LayoutAttribute, AttributeData> g_attributeMetadata =
{
	{ LayoutAttribute::Positions, AttributeData{3, sizeof(float), 0, typeid(float).name()}},
	{ LayoutAttribute::Normals, AttributeData{3, sizeof(float), 1, typeid(float).name()}},
	{ LayoutAttribute::Texcoords, AttributeData{2, sizeof(float), 2, typeid(float).name()}},
	{ LayoutAttribute::Colors, AttributeData{3, sizeof(float), 3, typeid(float).name()}},
	{ LayoutAttribute::Tangents, AttributeData{3, sizeof(float), 4, typeid(float).name()}},

	{ LayoutAttribute::InstanceModel_0, AttributeData{4, sizeof(float), 6, typeid(float).name()}},
	{ LayoutAttribute::InstanceModel_1, AttributeData{4, sizeof(float), 7, typeid(float).name()}},
	{ LayoutAttribute::InstanceModel_2, AttributeData{4, sizeof(float), 8, typeid(float).name()}},
	{ LayoutAttribute::InstanceModel_3, AttributeData{4, sizeof(float), 9, typeid(float).name()}},
};

AttributeData getAttributeData(LayoutAttribute attribute);

const std::map<LayoutAttribute, size_t> g_attributeToSizeMap =
{
	{ LayoutAttribute::Positions, sizeof(float) },
	{ LayoutAttribute::Normals, sizeof(float) },
	{ LayoutAttribute::Texcoords, sizeof(float) },
	{ LayoutAttribute::Colors, sizeof(float) },
	{ LayoutAttribute::Tangents, sizeof(float) },

	{ LayoutAttribute::InstanceModel_0, sizeof(float) },
	{ LayoutAttribute::InstanceModel_1, sizeof(float) },
	{ LayoutAttribute::InstanceModel_2, sizeof(float) },
	{ LayoutAttribute::InstanceModel_3, sizeof(float) },
};

size_t getAttributeSize(LayoutAttribute attribute);

const std::map<LayoutAttribute, size_t> g_attributeToCompCountMap =
{
	{ LayoutAttribute::Positions, 3	},
	{ LayoutAttribute::Normals, 3 },
	{ LayoutAttribute::Texcoords, 2 },
	{ LayoutAttribute::Colors, 3 },
	{ LayoutAttribute::Tangents, 3 },

	{ LayoutAttribute::InstanceModel_0, 4 },
	{ LayoutAttribute::InstanceModel_1, 4 },
	{ LayoutAttribute::InstanceModel_2, 4 },
	{ LayoutAttribute::InstanceModel_3, 4 },
};

size_t getAttributeCompCount(LayoutAttribute attribute);

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

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
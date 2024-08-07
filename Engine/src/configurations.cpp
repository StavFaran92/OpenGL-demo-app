#include "Configurations.h"

#include "Logger.h"

AttributeData getAttributeData(LayoutAttribute attribute)
{
	if (g_attributeMetadata.find(attribute) == g_attributeMetadata.end())
	{
		logError("Unsupported attribute specified.");
		return {};
	}

	return g_attributeMetadata.at(attribute);
}

size_t getAttributeSize(LayoutAttribute attribute)
{
	if (g_attributeToSizeMap.find(attribute) == g_attributeToSizeMap.end())
	{
		logError("Unsupported attribute specified.");
		return 0;
	}

	return g_attributeToSizeMap.at(attribute);
}

size_t getAttributeCompCount(LayoutAttribute attribute)
{
	if (g_attributeToCompCountMap.find(attribute) == g_attributeToCompCountMap.end())
	{
		logError("Unsupported attribute specified.");
		return 0;
	}

	return g_attributeToCompCountMap.at(attribute);
}

size_t getAttributeLocationInShader(LayoutAttribute attribute)
{
	if (g_attributeToLocationMap.find(attribute) == g_attributeToLocationMap.end())
	{
		logError("Unsupported attribute specified.");
		return 0;
	}

	return g_attributeToLocationMap.at(attribute);
}


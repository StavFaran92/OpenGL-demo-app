#include "Configurations.h"

#include "Logger.h"

size_t getAttributeSize(LayoutAttribute attribute)
{
	if (g_attributeToSizeMap.find(attribute) == g_attributeToSizeMap.end())
	{
		logError("Unsupported attribute specified.");
		return 0;
	}

	return g_attributeToSizeMap.at(attribute);
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


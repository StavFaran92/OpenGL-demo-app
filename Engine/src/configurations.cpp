#include "Configurations.h"

#include "Logger.h"

size_t getAttributeSize(LayoutAttributes attribute)
{
	if (g_attributeToSizeMap.find(attribute) == g_attributeToSizeMap.end())
	{
		logError("Unsupported attribute: ");
		return 0;
	}

	return g_attributeToSizeMap.at(attribute);
}


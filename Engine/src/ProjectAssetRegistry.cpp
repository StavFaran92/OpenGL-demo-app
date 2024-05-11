#include "ProjectAssetRegistry.h"

void ProjectAssetRegistry::create()
{
	// create a new registry file
}

void ProjectAssetRegistry::reset()
{

	// reset the asset registry file
}

void ProjectAssetRegistry::addMesh(UUID uuid)
{
	// add mesh uid to file

	// write mesh data to binary resource dir ??
}

void ProjectAssetRegistry::addTexture(UUID uuid)
{
}

std::vector<UUID> ProjectAssetRegistry::getMeshList() const
{
	return std::vector<UUID>();
}

std::vector<UUID> ProjectAssetRegistry::getTextureList() const
{
	return std::vector<UUID>();
}

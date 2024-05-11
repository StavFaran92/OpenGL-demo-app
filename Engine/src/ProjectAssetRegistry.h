#pragma once

#include <vector>

#include "UUID.h"

class ProjectAssetRegistry
{
public:
	void create();
	void reset();
	void load(); //????
	void addMesh(UUID uuid);
	void addTexture(UUID uuid);
	std::vector<UUID> getMeshList() const;
	std::vector<UUID> getTextureList() const;
};
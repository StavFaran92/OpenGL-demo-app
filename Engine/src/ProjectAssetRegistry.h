#pragma once

#include <vector>

#include <nlohmann/json.hpp>
#include "UUID.h"

class ProjectAssetRegistry
{
public:
	ProjectAssetRegistry(const std::string& filename);
	static std::shared_ptr<ProjectAssetRegistry> create(const std::string& filename);
	static std::shared_ptr<ProjectAssetRegistry> parse(const std::string& filename);

	std::string getFilepath() const;
	void reset();
	void load(); //????
	void save();
	void addMesh(UUID uuid);
	void addTexture(UUID uuid);
	std::vector<UUID> getMeshList() const;
	std::vector<UUID> getTextureList() const;


private:
	
	void sync();

private:
	std::string m_filename;
	nlohmann::json m_assetRegistry;
};
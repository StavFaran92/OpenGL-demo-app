#include "ProjectAssetRegistry.h"

#include "Logger.h"


#include <filesystem>
#include <fstream>

using json = nlohmann::json;

ProjectAssetRegistry::ProjectAssetRegistry(const std::string& filename)
    : m_filename(filename)
{

}

std::shared_ptr<ProjectAssetRegistry> ProjectAssetRegistry::create(const std::string& filename)
{
    std::shared_ptr<ProjectAssetRegistry> par = std::make_shared<ProjectAssetRegistry>(filename);

    // Create JSON object with empty arrays for meshes and textures
    par->m_assetRegistry["meshes"] = nlohmann::json::array();
    par->m_assetRegistry["textures"] = nlohmann::json::array();

    // Write JSON data to file
    std::ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        logError("Failed to create asset registry file: " + filename);
        return nullptr;
    }
    outputFile << std::setw(4) << par->m_assetRegistry << std::endl;
    outputFile.close();

    // Log success
    logInfo("Asset registry file created: " + filename);

    return par;
}

std::shared_ptr<ProjectAssetRegistry> ProjectAssetRegistry::parse(const std::string& filename)
{
    std::shared_ptr<ProjectAssetRegistry> par = std::make_shared<ProjectAssetRegistry>(filename);

    std::ifstream outputFile(filename);
    if (!outputFile.is_open())
    {
        logError("Failed to open asset registry file: " + filename);
        return nullptr;
    }

    std::string file_content((std::istreambuf_iterator<char>(outputFile)), std::istreambuf_iterator<char>());

    auto resourceFile = json::parse(file_content, nullptr, false);

    if (resourceFile.is_discarded())
    {
        logError("Failed to load json file");
        return nullptr;
    }

    {
        auto iter = resourceFile.find("meshes");
        if (iter == resourceFile.end())
        {
            logError("Could not find \"meshes\" in JSON");
            return nullptr;
        }

        // Iterate over the "meshes" array
        for (const auto& mesh : iter.value())
        {
            try
            {
                std::string meshFileName = mesh.get<std::string>();
                par->addMesh(meshFileName);
            }
            catch (const std::exception& e)
            {
                logError("Failed to parse mesh value: " + std::string(e.what()));
                continue; // Continue to the next mesh
            }
        }
    }

    {
        auto iter = resourceFile.find("textures");
        if (iter == resourceFile.end())
        {
            logError("Could not find \"textures\" in JSON");
            return nullptr;
        }

        for (const auto& texture : iter.value())
        {
            try
            {
                std::string textureFileName = texture.get<std::string>();
                par->addTexture(textureFileName);
            }
            catch (const std::exception& e)
            {
                logError("Failed to parse texture value: " + std::string(e.what()));
                continue; // Continue to the next mesh
            }
        }
    }

    return par;
}

void ProjectAssetRegistry::reset()
{

	// reset the asset registry file
}

void ProjectAssetRegistry::save()
{
    sync();
}

void ProjectAssetRegistry::addMesh(UUID uuid)
{
    m_assetRegistry["meshes"].push_back(uuid);
}

void ProjectAssetRegistry::addTexture(UUID uuid)
{
    m_assetRegistry["textures"].push_back(uuid);
}

std::vector<UUID> ProjectAssetRegistry::getMeshList() const
{
    return m_assetRegistry["meshes"].get<const std::vector<UUID>>();
}

std::vector<UUID> ProjectAssetRegistry::getTextureList() const
{
	return m_assetRegistry["textures"].get<std::vector<std::string>>();
}

void ProjectAssetRegistry::sync()
{
    // Open file in write mode to clear its contents
    std::ofstream outputFile(m_filename, std::ios::trunc);
    if (!outputFile.is_open()) 
    {
        logError("Failed to open asset registry file: " + m_filename);
        return;
    }

    // Write JSON data to file
    outputFile << std::setw(4) << m_assetRegistry << std::endl;
    outputFile.close();
}

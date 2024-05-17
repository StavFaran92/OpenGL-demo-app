#include "ProjectManager.h"

#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "Logger.h"
#include "Mesh.h"
#include "ProjectAssetRegistry.h"
#include "MeshSerializer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace fs = std::filesystem;

void ProjectManager::loadProject(const std::string& filePath, std::shared_ptr<Context>& context)
{
    
    // Check if the file exists
    if (!fs::exists(filePath)) 
    {
        logError("File not found: " + filePath);
        return;
    }

    // Open file
    std::ifstream projectFile(filePath);
    if (!projectFile.is_open())
    {
        logError("Failed to open file: " + filePath);
        return ;
    }

    // Get file directory
    fs::path projectDir = fs::path(filePath).parent_path();

    // open scene registry file
    std::string ecsFilepath = (projectDir / "entities.json").string();

    // Open file
    std::ifstream ecsFile(ecsFilepath);
    if (!ecsFile.is_open())
    {
        logError("Failed to open file: " + ecsFilepath);
        return;
    }

    // Parse JSON
    json ecsJson;
    try 
    {
        ecsJson = json::parse(ecsFile);
    }
    catch (const std::exception& e) {
        logError("Failed to parse JSON: " + std::string(e.what()));
        return;
    }

    // Parse JSON
    json resourceFileJSON;
    try 
    {
        resourceFileJSON = json::parse(projectFile);
    }
    catch (const std::exception& e) 
    {
        logError("Failed to parse JSON: " + std::string(e.what()));
        return ;
    }

    auto par = context->getProjectAssetRegistry();

    std::vector<std::string> meshNameList = par->getMeshList();
    std::vector<std::string> textureNameList;

    // Create meshes
    for (const auto& meshUID : meshNameList) 
    {
        // Open bin file
        fs::path binFilePath = (projectDir / meshUID).string() + ".bin";
        std::ifstream binFile(binFilePath, std::ios::binary);
        if (!binFile.is_open()) 
        {
            logError("Failed to open bin file for mesh: " + meshUID + ".bin");
            continue; // Skip this mesh
        }
        MeshData meshData;
        MeshSerializer::readDataFromBinaryFile(binFilePath.string(), meshData);

        // Create mesh
        Mesh* mesh = new Mesh();
        mesh->build(meshData);
        Engine::get()->getMemoryPool<Mesh>()->add(meshUID, mesh);
    }

    // Create textures
    for (const auto& texture : textureNameList) {
        // Similar steps as above to load and create textures
        // ...
    }

    // Populate scenes from JSON
    context->populateScenesFromJSON(ecsJson.dump()); // Pass JSON as string
}

void ProjectManager::saveProject()
{
    Engine::get()->getContext()->getProjectAssetRegistry()->save();

    Engine::get()->getContext()->getActiveScene()->serialize();
}

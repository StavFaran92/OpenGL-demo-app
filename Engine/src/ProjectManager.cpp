#include "ProjectManager.h"

#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "Logger.h"
#include "Mesh.h"
#include "MeshSerializer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace fs = std::filesystem;

std::shared_ptr<Context> ProjectManager::loadProject(const std::string& filePath)
{
    
    // Check if the file exists
    if (!fs::exists(filePath)) 
    {
        logError("File not found: " + filePath);
        return nullptr;
    }

    // Open file
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        logError("Failed to open file: " + filePath);
        return nullptr;
    }

    // Get file directory
    fs::path projectDir = fs::path(filePath).parent_path();

    // open scene registry file
    std::string ecsFilepath = filePath + "/ecs.json";

    // Open file
    std::ifstream ecsFile(ecsFilepath);
    if (!ecsFile.is_open())
    {
        logError("Failed to open file: " + ecsFilepath);
        return nullptr;
    }

    // Read file content into a string
    std::string fileContent((std::istreambuf_iterator<char>(ecsFile)), std::istreambuf_iterator<char>());

    // Parse JSON
    json ecsJson;
    try 
    {
        ecsJson = json::parse(fileContent);
    }
    catch (const std::exception& e) {
        logError("Failed to parse JSON: " + std::string(e.what()));
        return nullptr;
    }

    // Create context
    std::shared_ptr<Context> context = std::make_shared<Context>();

    // Populate scenes from JSON
    context->populateScenesFromJSON(ecsJson.dump()); // Pass JSON as string

    // Parse JSON
    json resourceFileJSON;
    try 
    {
        resourceFileJSON = json::parse(file);
    }
    catch (const std::exception& e) 
    {
        logError("Failed to parse JSON: " + std::string(e.what()));
        return nullptr;
    }

    auto iter = resourceFileJSON.find("meshes");
    if (iter == resourceFileJSON.end())
    {
        logError("Could not find \"Meshes\" in JSON");
        return nullptr;
    }

    std::vector<std::string> meshNameList;
    std::vector<std::string> textureList;

    // Iterate over the "meshes" array
    for (const auto& mesh : *iter)
    {
        try
        {
            std::string meshFileName = mesh.get<std::string>();
            meshNameList.push_back(meshFileName);
        }
        catch (const std::exception& e)
        {
            logError("Failed to parse mesh value: " + std::string(e.what()));
            continue; // Continue to the next mesh
        }
    }

    // Create meshes
    for (const auto& meshUID : meshNameList) 
    {
        // Open bin file
        fs::path binFilePath = (projectDir / "bin" / meshUID).string() + ".bin";
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
        Engine::get()->getMemoryPool<Mesh>()->add(meshUID, mesh);
    }

    // Create textures
    for (const auto& texture : textureList) {
        // Similar steps as above to load and create textures
        // ...
    }

    // Return context
    return context;
}

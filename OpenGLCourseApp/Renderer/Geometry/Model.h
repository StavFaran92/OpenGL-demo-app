#pragma once

#include <vector>
#include <string>

#include "Renderer/Shader/Shader.h"
#include "Renderer/Geometry/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ApplicationConstants.h"

class Model
{
public:
    Model(const std::string path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader, const Renderer& renderer);
private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
private:
    // model data
    std::vector<Mesh> m_meshes;
    std::string m_modelDir;
    std::vector<Texture> m_texturesCache;
};
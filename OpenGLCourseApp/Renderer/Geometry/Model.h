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
        std::cout << __FUNCTION__ << std::endl;

        loadModel(path);
    }
    ~Model() {
        std::cout << __FUNCTION__ << std::endl;
    }
    void Draw(Shader& shader, const Renderer& renderer);
private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
private:
    // model data
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::string m_modelDir;
    std::vector<std::shared_ptr<Texture>> m_texturesCache;
};
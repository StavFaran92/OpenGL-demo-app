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
    Model(const std::string& path)
        :m_path(path)
    {
        std::cout << __FUNCTION__ << std::endl;
        m_modelDir = m_path.substr(0, m_path.find_last_of('\\'));
    }
    ~Model() {
        std::cout << __FUNCTION__ << std::endl;
    }
    void loadModel();
    void Draw(Shader& shader, const Renderer& renderer);

    void Update(float delta);
private:
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
private:
    // model data
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::string m_modelDir = "";
    std::vector<std::shared_ptr<Texture>> m_texturesCache;
    const std::string m_path = "";
};
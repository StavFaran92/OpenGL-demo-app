#pragma once

#include <vector>
#include <string>
#include <map>

#include "Renderer/Shader/Shader.h"
#include "Renderer/Geometry/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ApplicationConstants.h"
#include "Core/Application.h"
#include "Renderer/Lighting/DirectionalLight.h"
#include "Renderer/Shader/Material.h"

#include "Transform.h"

class Model
{
public:
	Model(const std::string& path);
	~Model() {
		logTrace( __FUNCTION__ );
	}
	void loadModel();
	void Draw(std::shared_ptr<Renderer> renderer);

	bool AttachShader(std::shared_ptr<Shader > shader);
	bool DetachShader();

	bool UseLight(std::shared_ptr<Light> light);
	bool UseMaterial(std::shared_ptr<Material> material);

	bool UseShader();

	std::shared_ptr<Shader> GetShader();
	void FlipTexture(bool flip);

	void Update(float delta);
	std::shared_ptr<Transform> GetTransformation() const { return transformation; }
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

	std::shared_ptr<Transform> transformation;

	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<Light> m_light = nullptr;
	std::shared_ptr<Material> m_material = nullptr;
	bool m_flipTexture = false;
};
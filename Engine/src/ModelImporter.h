#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"

// Forward declerations
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace Assimp
{
	class Importer;
}

class ModelImporter
{
public:
	struct ModelImportSession
	{
		bool flippedTexture = false;
		std::string filepath;
		std::string fileDir;

	};

	/** Constructor */
	ModelImporter();

	/** Init the model loader module */
	void init();

	/**
	 * Import a model from a file.
	 *
	 * \param path			path to the given file
	 * \param flipTexture	should flip loaded texture
	 * \return A poitner to the newly created model
	 */
	Model* loadModelFromFile(const std::string& path, bool flipTexture = false);
private:
	void processNode(aiNode* node, const aiScene* scene, ModelImportSession& session, Model& model);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene, ModelImportSession& session);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, ModelImportSession& session);
	static Texture::Type getTextureType(aiTextureType type);
private:
	std::vector<std::shared_ptr<Texture>> m_texturesCache;
	std::map<uint32_t, ModelImportSession> m_sessions;
	std::shared_ptr<Assimp::Importer> m_importer = nullptr;
};

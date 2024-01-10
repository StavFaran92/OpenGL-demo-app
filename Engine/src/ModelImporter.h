#pragma once

#include <unordered_map>
#include <map>
#include <memory>
#include <string>

#include "Core.h"
#include "Mesh.h"
#include "Texture.h"
#include "Entity.h"

// Forward declerations
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;
class Engine;
class TextureHandler;
class Scene;
template<typename T>class ObjectHandler;

namespace Assimp
{
	class Importer;
}

class EngineAPI ModelImporter
{
public:
	struct ModelImportSession
	{
		std::string filepath;
		std::string fileDir;
		int nodeIndex = 0;
		int childIndex = 0;
	};

	/** Constructor */
	ModelImporter();

	/**
	 * Import a model from a file.
	 *
	 * \param path			path to the given file
	 * \param flipTexture	should flip loaded texture
	 * \return A poitner to the newly created model
	 */
	Entity loadModelFromFile(const std::string& path, Scene* pScene);
private:
	friend class Engine;
	/** Init the model loader module */
	void init();
	void processNode(aiNode* node, const aiScene* scene, ModelImportSession& session, Entity entity, Scene* pScene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene, ModelImportSession& session);
	std::vector<TextureHandler*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, ModelImportSession& session);
	static Texture::Type getTextureType(aiTextureType type);
private:
	//std::unordered_map<std::string, std::weak_ptr<Texture>> m_texturesCache;
	std::map<uint32_t, ModelImportSession> m_sessions;
	std::shared_ptr<Assimp::Importer> m_importer = nullptr;

	std::string m_lastLoadedSceneName;
};

#pragma once

// STL
#include <vector>
#include <string>
#include <map>

// sge
#include "Core.h"
#include "Object3D.h"
#include "Shader.h"
#include "Transform.h"
#include "Material.h"
#include "Texture.h"

// Forward declerations
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

struct Vertex;
class IRenderer;
class Mesh;
class Scene;


class EngineAPI Model : public Object3D
{
public:
	enum class PrimitiveType 
	{
		Quad,
		Cube,
		Sphere

	};
public:
	// -------------------- Methods -------------------- //
	/** Constructor */
	Model();

	/** Destructor */
	~Model();

	/**
	 * Load a model from a file.
	 * 
	 * \param path			path to the given file
	 * \param flipTexture	should flip loaded texture
	 * \return A poitner to the newly created model
	 */
	static Model* loadModelFromFile(const std::string& path, bool flipTexture=false);

	///**
	// * Create a primitive shape model.
	// * 
	// * \param ptype	primitive type to create
	// * \return A poitner to the newly created model
	// */
	//static Model* createPrimitiveModel(PrimitiveType ptype);

	

	bool AttachShader(std::shared_ptr<Shader > shader);
	bool DetachShader();

	bool UseMaterial(std::shared_ptr<Material> material);

	bool UseShader();

	std::shared_ptr<Shader> GetShader();

	

	std::vector<std::shared_ptr<Texture>> GetTextures();

	void FlipTexture(bool flip);
	inline void SetReflection(bool val) { m_isReflective = val; }
	inline void SetRefraction(bool val) { m_isRefractive = val; }
private:
	friend class Scene;
	void update(float deltaTime);
	void processNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
	static Texture::Type getTextureType(aiTextureType type);
protected:
	virtual void Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader = nullptr);
	
	// -------------------- Attributes -------------------- //
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::string m_modelDir = "";
	std::vector<std::shared_ptr<Texture>> m_texturesCache;
	std::string m_path = "";
	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<Material> m_material = nullptr;
	bool m_flipTexture = false;

	bool m_isReflective = false;
	bool m_isRefractive = false;
};
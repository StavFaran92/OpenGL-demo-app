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
class ModelImporter;
class MeshBuilder;
class ModelBuilder;

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

	bool AttachShader(std::shared_ptr<Shader > shader);
	bool DetachShader();

	bool UseMaterial(std::shared_ptr<Material> material);

	bool use();

	Shader* GetShader();

	std::vector<std::shared_ptr<Texture>> GetTextures();

	void flipTexture();

	inline void SetReflection(bool val) { m_isReflective = val; }
	inline void SetRefraction(bool val) { m_isRefractive = val; }

	virtual MeshBuilder* createMeshBuilder();

	//template<typename T>
	//static ModelBuilder<T>* builder()
	//{
	//	return ModelBuilder<Model>();
	//}

private:
	friend class Scene;
	friend class ModelImporter;
	friend class ModelBuilder;

	void update(float deltaTime);
	void addMesh(Mesh* mesh);
	
protected:
	// -------------------- Methods -------------------- //
	virtual void Draw(IRenderer& renderer, Shader* shader = nullptr);
	
	// -------------------- Attributes -------------------- //
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<Material> m_material = nullptr;

	bool m_isReflective = false;
	bool m_isRefractive = false;
};
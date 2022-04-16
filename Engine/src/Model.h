#pragma once

// STL
#include <vector>
#include <string>
#include <map>

// sge
#include "Core.h"
#include "Object3D.h"
#include "Shader.h"
#include "Transformation.h"
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
	// -------------------- Methods -------------------- //
	/** Constructor */
	Model();

	/** Destructor */
	~Model();	

	/**
	 * Attach a shader to the model.
	 * 
	 * The attached shader will be used to render the mesh with.
	 * 
	 * \param shader	A pointer to the shader to be attached
	 * \return			True if the attach was succesfull, False otherwise
	 */
	bool attachShader(Shader* shader);

	/**
	 * Attach a shader to the model.
	 *
	 * The attached shader will be used to render the mesh with.
	 *
	 * \param shader	A pointer to the shader to be attached
	 * \return			True if the attach was succesful, False otherwise
	 */
	bool attachShader(std::shared_ptr<Shader> shader);

	/**
	 * Detach a shader from the model.
	 *
	 * Detach the currently attached shader from the model,
	 * a default shader will be used to render the model if a draw call is issued.
	 *
	 * \return			True if the detach was succesful, False otherwise
	 */
	bool detachShader();

	bool useMaterial(std::shared_ptr<Material> material);

	/**
	 * Get the currently attached shader.
	 *
	 * \return	The currently attached shader
	 */
	Shader* getShader();

	std::vector<const Texture*> getTextures();

	void setReflection(bool enable);
	void setRefraction(bool enable);

	virtual MeshBuilder* createMeshBuilder();

	virtual void draw(IRenderer& renderer, Shader* shader = nullptr);
protected:
	// -------------------- Friends -------------------- //
	friend class Scene;
	friend class ModelImporter;
	friend class ModelBuilder;

	virtual void update(float deltaTime);
	void addMesh(Mesh* mesh);
	
protected:
	// -------------------- Methods -------------------- //
	//virtual void draw(IRenderer& renderer, Shader* shader = nullptr);
	
	// -------------------- Attributes -------------------- //
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<Material> m_material = nullptr;

	bool m_isReflective = false;
	bool m_isRefractive = false;
};
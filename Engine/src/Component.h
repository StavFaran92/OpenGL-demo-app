#pragma once

#include <memory>

#include "Core.h"

#include "ScriptableEntity.h"
#include "Configurations.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Physics.h"

class Scene;
class Mesh;


struct EngineAPI Component
{

};

struct EngineAPI TagComponent : public Component
{
	std::string tag;
};

struct EngineAPI SkyboxComponent : public Component
{
	SkyboxComponent() = default;

	SkyboxComponent(Resource<Texture> skyboxImage) : skyboxImage(skyboxImage) {}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(originalImage);
	}

	Resource<Texture> skyboxImage;
	Resource<Texture> originalImage;
};

struct EngineAPI RenderableComponent : public Component
{
	RenderableComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(renderTechnique);
	}

	enum class RenderTechnique : int
	{
		Forward,
		Deferred
	};

	RenderTechnique renderTechnique = RenderTechnique::Deferred;
};

struct EngineAPI NativeScriptComponent : public Component
{
	NativeScriptComponent() = default;
	std::shared_ptr<ScriptableEntity> script = nullptr;
	Entity entity = Entity::EmptyEntity;

	template<typename T, typename... Args>
	T* bind(Args&&... args)
	{
		static_assert(std::is_base_of<ScriptableEntity, T>::value, "T must inherit from ScriptableEntity");

		script = std::make_shared< T>(std::forward<Args>(args)...);
		script->entity = entity;
		script->onCreate();

		return static_cast<T*>(script.get());
	}

	void unbind()
	{
		if (script)
		{
			//delete script;
			script = nullptr;
		}
	}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(entity, script);
	}
};

struct EngineAPI RigidBodyComponent : public Component
{

	RigidBodyComponent() = default;
	RigidBodyComponent(RigidbodyType type, float mass) : type(type), mass(mass) {};

	void addForce(glm::vec3 force);
	void setForce(glm::vec3 force);

	void move(glm::vec3 position);

	template <class Archive>
	void serialize(Archive& archive) {
		archive(type, mass);
	}

	bool isLockedLinearX = false;
	bool isLockedLinearY = false;
	bool isLockedLinearZ = false;
	bool isLockedAngularX = false;
	bool isLockedAngularY = false;
	bool isLockedAngularZ = false;
	RigidbodyType type = RigidbodyType::Static;
	float mass = 0;
	bool isChanged = false;
	glm::vec3 m_targetPisition{0};
	glm::vec3 m_force{ 0 };
	void* simulatedBody = nullptr;
};

struct EngineAPI CollisionBoxComponent : public Component
{
	CollisionBoxComponent() = default;
	CollisionBoxComponent(float halfExtent) : halfExtent(halfExtent) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(halfExtent, layerMask);
	}

	float halfExtent = 0;
	Physics::LayerMask layerMask = Physics::LayerMask::LAYER_0;
};

struct EngineAPI CollisionSphereComponent : public Component
{
	CollisionSphereComponent() = default;
	CollisionSphereComponent(float radius) : radius(radius) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(radius, layerMask);
	}

	float radius = 0;
	Physics::LayerMask layerMask = Physics::LayerMask::LAYER_0;
};

struct EngineAPI CollisionMeshComponent : public Component
{
	CollisionMeshComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(isConvex, layerMask);
	}

	bool isConvex = false;
	Resource<Mesh> mesh = Resource<Mesh>::empty;
	Physics::LayerMask layerMask = Physics::LayerMask::LAYER_0;
};

struct EngineAPI CameraComponent : public Component
{
	CameraComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(center, up);
	}

	static CameraComponent createPerspectiveCamera(float fovy, float aspect, float znear, float zfar)
	{
		CameraComponent cam;
		cam.fovy = fovy;
		cam.aspect = aspect;
		cam.znear = znear;
		cam.zfar = zfar;
		cam.type = CamType::PERSPECTIVE;
		return cam;
	}

	glm::mat4 getProjection() const;

	float fovy = 0;
	float aspect = 0;
	float znear = 0;
	float zfar = 0;

	enum CamType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	CamType type;

	glm::vec3 front{0,0,-1};
	glm::vec3 right;
	glm::vec3 center{ 0,0,0 };
	glm::vec3 up{ 0,1,0 };
};

struct EngineAPI MeshComponent : public Component
{
	MeshComponent(const Resource<MeshCollection>& mesh) : mesh(mesh) {};
	MeshComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(mesh, materialSlot);
	}

	float materialSlot = 0; // todo this will be used (probably as a list) to support multi material models
	Resource<MeshCollection> mesh = Resource<MeshCollection>::empty;
};

//struct EngineAPI MeshArrayRendererComponent : public Component
//{
//	MeshArrayRendererComponent() = default;
//	
//	void addMesh(const MeshRenderer& mesh)
//	{
//		meshes.push_back(mesh);
//	}
//
//	auto begin() { return meshes.begin(); }
//	auto end() { return meshes.end(); }
//	auto begin() const { return meshes.begin(); }
//	auto end() const { return meshes.end(); }
//
//	template <class Archive>
//	void serialize(Archive& archive) {
//		archive(meshes);
//	}
//
//	std::vector<MeshRenderer> meshes;
//};

struct EngineAPI MaterialComponent : public Component
{
	MaterialComponent();
	

	auto begin() { return materials.begin(); }
	auto end() { return materials.end(); }
	auto begin() const { return materials.begin(); }
	auto end() const { return materials.end(); }

	void addMaterial(const std::shared_ptr<Material>& mat)
	{
		materials[count++] = mat;
	}

	void setMaterial(int index, const std::shared_ptr<Material>& mat)
	{
		materials[index] = mat;
	}

	std::shared_ptr<Material> at(int index)
	{
		return materials.at(index);
	}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(materials);
	}

	std::map<int, std::shared_ptr<Material>> materials;
	int count = 0;
};

struct EngineAPI ObjectComponent : public Component
{
	ObjectComponent() = default;
	ObjectComponent(Entity e, const std::string& name) : name(name), e(e) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(name, e);
	}

	std::string name;
	Entity e = Entity::EmptyEntity;
};

struct EngineAPI ShaderComponent : public Component
{
	ShaderComponent() = default;
	ShaderComponent(Shader* vertexShader, Shader* fragmentShader) : m_vertexShader(vertexShader), m_fragmentShader(fragmentShader) {};

	void addTexture(const std::string& name, Resource<Texture> texture)
	{
		customTextures[name] = texture;
	}

	//template <class Archive>
	//void serialize(Archive& archive) {
	//	archive(m_shader);
	//}

	Shader* m_vertexShader = nullptr;
	Shader* m_fragmentShader = nullptr;

	std::map<std::string, Resource<Texture>> customTextures;
};

struct EngineAPI InstanceBatch : public Component
{
public:
	InstanceBatch();

	InstanceBatch(const std::vector< std::shared_ptr<Transformation>>& transformations, Resource<Mesh> mesh);
	

	void addTransformation(const std::shared_ptr<Transformation>& transformation);

	const std::vector<glm::mat4> getMatrices() const;
	std::vector<std::shared_ptr<Transformation>>& getTransformations();
	
	size_t getCount() const
	{
		return transformations.size();
	}

	void build();
private:
	

private:
	std::vector<std::shared_ptr<Transformation>> transformations;
	Resource<Mesh> mesh;
	unsigned int m_id = 0;
};

struct EngineAPI ImageComponent : public Component
{
	ImageComponent() = default;

	ImageComponent(Resource<Texture> image) : image(image) {}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(image, size, position, rotate);
	}

	glm::vec2 size;
	glm::vec2 position;
	float rotate = 0;

	Resource<Texture> image;
};

struct CharacterController : public Component
{
	CharacterController() = default;


};
#pragma once

#include <memory>

#include "Core.h"

#include "ScriptableEntity.h"
#include "Configurations.h"
#include <glm/gtc/matrix_transform.hpp>

class Scene;
class Mesh;


struct EngineAPI Component
{

};

struct EngineAPI TagComponent : public Component
{
	[[maybe_unused]] bool empty = false;
};

struct EngineAPI SkyboxComponent : public TagComponent
{
	float t; // todo fix

	template <class Archive>
	void serialize(Archive& archive) {
		archive(t);
	}
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

	template <class Archive>
	void serialize(Archive& archive) {
		archive(type, mass);
	}

	RigidbodyType type = RigidbodyType::Static;
	float mass = 0;
	void* simulatedBody = nullptr;
};

struct EngineAPI CollisionBoxComponent : public Component
{
	CollisionBoxComponent() = default;
	CollisionBoxComponent(float halfExtent) : halfExtent(halfExtent) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(halfExtent);
	}

	float halfExtent = 0;
};

struct EngineAPI CollisionSphereComponent : public Component
{
	CollisionSphereComponent() = default;
	CollisionSphereComponent(float radius) : radius(radius) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(radius);
	}

	float radius = 0;
};

struct EngineAPI CollisionMeshComponent : public Component
{
	CollisionMeshComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(isConvex);
	}

	bool isConvex = false;
	Mesh* mesh = nullptr;
};

struct EngineAPI CameraComponent : public Component
{
	CameraComponent() = default;
	glm::mat4 getView()
	{
		return glm::lookAt(position, center, up);
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(position, center, up, isPrimary);
	}

	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;

	bool isPrimary = false;
};

struct EngineAPI MeshComponent : public Component
{
	MeshComponent(const Resource<Mesh>& mesh) : mesh(mesh) {};
	MeshComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(mesh, materialSlot);
	}

	float materialSlot = 0; // todo this will be used (probably as a list) to support multi material models
	Resource<Mesh> mesh;
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
		materials.push_back(mat);
	}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(materials);
	}

	std::vector<std::shared_ptr<Material>> materials;
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
	ShaderComponent(Shader* shader) : m_shader(shader) {};

	//template <class Archive>
	//void serialize(Archive& archive) {
	//	archive(m_shader);
	//}

	Shader* m_shader = nullptr;
};

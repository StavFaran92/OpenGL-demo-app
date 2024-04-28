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

struct EngineAPI TagComponent
{
	[[maybe_unused]] bool empty = false;
};

struct EngineAPI SkyboxComponent : public TagComponent
{
	
};

struct EngineAPI RenderableComponent : public Component
{
	enum RenderTechnique
	{
		Forward,
		Deferred
	};

	RenderTechnique renderTechnique = RenderTechnique::Deferred;
};

struct EngineAPI NativeScriptComponent : public Component
{
	ScriptableEntity* script = nullptr;
	Entity entity = Entity::EmptyEntity;

	template<typename T, typename... Args>
	T* bind(Args&&... args)
	{
		static_assert(std::is_base_of<ScriptableEntity, T>::value, "T must inherit from ScriptableEntity");

		script = new T(std::forward<Args>(args)...);
		script->entity = entity;
		script->onCreate();

		return static_cast<T*>(script);
	}

	void unbind()
	{
		if (script)
		{
			delete script;
			script = nullptr;
		}
	}
};

struct EngineAPI RigidBodyComponent : public Component
{
	RigidBodyComponent(RigidbodyType type, float mass) : type(type), mass(mass) {};

	RigidbodyType type = RigidbodyType::Static;
	float mass = 0;
	void* simulatedBody = nullptr;
};

struct EngineAPI CollisionBoxComponent : public Component
{
	CollisionBoxComponent(float halfExtent) : halfExtent(halfExtent) {};

	float halfExtent = 0;
};

struct EngineAPI CollisionSphereComponent : public Component
{
	CollisionSphereComponent(float radius) : radius(radius) {};

	float radius = 0;
};

struct EngineAPI CollisionMeshComponent : public Component
{
	bool isConvex = false;
	Mesh* mesh = nullptr;
};

struct EngineAPI CameraComponent : public Component
{
	glm::mat4 getView()
	{
		return glm::lookAt(position, center, up);
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;
};

struct EngineAPI MeshComponent : public Component
{
	MeshComponent(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}

	std::shared_ptr<Mesh> mesh = nullptr;
};

struct EngineAPI ObjectComponent : public Component
{
	ObjectComponent() = delete;
	ObjectComponent(Entity e, const std::string& name) : name(name), e(e) {};

	std::string name;
	Entity e;
};

#pragma once

#include "Core.h"

#include "ScriptableEntity.h"
#include "Configurations.h"

class Scene;


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

struct EngineAPI RenderableComponent : public TagComponent
{
	
};

struct EngineAPI HierarchyComponent : public Component
{
	Entity parent;
	std::unordered_map<entity_id,Entity> children{};
	Scene* scene = nullptr;
};

struct EngineAPI NativeScriptComponent : public Component
{
	ScriptableEntity* script = nullptr;
	std::function<ScriptableEntity*()> instantiateScript = nullptr;

	template<typename T, typename... Args>
	void bind(Args&&... args)
	{
		static_assert(std::is_base_of<ScriptableEntity, T>::value, "T must inherit from ScriptableEntity");

		instantiateScript = [args...]() {
			return static_cast<ScriptableEntity*>(new T(std::forward<Args>(args)...));
		};
	}

	void destroyScript()
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
	RigidbodyType type = RigidbodyType::Static;
	float mass = 0;
	void* simulatedBody = nullptr;
};

struct EngineAPI CollisionBoxComponent : public Component
{
	float halfExtent = 0;
};

struct EngineAPI CollisionSphereComponent : public Component
{
	float radius = 0;
};

struct EngineAPI CameraComponent : public Component
{
	ICamera* camera = nullptr;
};
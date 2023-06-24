#pragma once

#include "Core.h"

#include "ScriptableEntity.h"

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
	entt::entity parent{ entt::null };
	std::vector<entt::entity> children{};
	Scene* scene = nullptr;
};

struct EngineAPI NativeScriptComponent : public Component
{
	ScriptableEntity* script = nullptr;
	std::function<ScriptableEntity*()> instantiateScript = nullptr;

	template<typename T>
	void bind()
	{
		static_assert(std::is_base_of<ScriptableEntity, T>::value, "T must inherit from ScriptableEntity");
		instantiateScript = [](){ return static_cast<ScriptableEntity*>( new T()); };
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
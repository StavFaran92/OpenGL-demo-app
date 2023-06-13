#pragma once

#include "Core.h"

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
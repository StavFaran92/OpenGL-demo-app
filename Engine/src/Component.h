#pragma once

#include "Core.h"

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

struct EngineAPI ParentComponent : public Component
{
	entt::entity parent{ entt::null };
};
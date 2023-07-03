#pragma once

#include "Core.h"

#include "Entity.h"

class Scene;

class EngineAPI ScriptableEntity
{
public:
	virtual ~ScriptableEntity() = default;

	Entity entity;

protected:
	virtual void onCreate() {};
	virtual void onUpdate(float deltaTime) {};
	virtual void onDestroy() {};

	friend class Scene;

protected:

};
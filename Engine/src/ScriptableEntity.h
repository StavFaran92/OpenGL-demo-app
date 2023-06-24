#pragma once

#include "Core.h"

#include "Entity.h"

class Scene;

class EngineAPI ScriptableEntity
{
public:
	virtual ~ScriptableEntity() = default;

protected:
	virtual void onCreate() {};
	virtual void onUpdate(float deltaTime) {};
	virtual void onDestroy() {};

	Entity entity;

	friend class Scene;

protected:

};
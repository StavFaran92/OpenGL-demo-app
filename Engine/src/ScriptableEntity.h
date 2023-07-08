#pragma once

#include "Core.h"

#include "Entity.h"

struct NativeScriptComponent;

class EngineAPI ScriptableEntity
{
public:
	virtual ~ScriptableEntity() = default;

	Entity entity;

public:
	virtual void onCreate() {};
	virtual void onUpdate(float deltaTime) {};
	virtual void onDestroy() {};

	friend struct NativeScriptComponent;

protected:
	bool m_isInit = false;
};
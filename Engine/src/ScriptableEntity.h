#pragma once

#include "Core.h"

#include "Entity.h"

class EngineAPI ScriptableEntity
{
public:
	virtual void onCreate() {};
	virtual void onUpdate(float deltaTime) {};
	virtual void onDestroy() {};

	virtual ~ScriptableEntity() = default;
private:

};
#pragma once

#include "Core.h"

#include "Entity.h"

struct NativeScriptComponent;
class Scene;
class SceneSerializer;

class EngineAPI ScriptableEntity
{
public:
	ScriptableEntity() 
	{
		name = typeid(*this).name();
	}
	virtual ~ScriptableEntity() = default;

	Entity entity;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(entity);
	}

	std::string name;

protected:
	virtual void onCreate() {};
	virtual void onUpdate(float deltaTime) {};
	virtual void onDestroy() {};

	friend NativeScriptComponent;
	friend Scene;
	friend SceneSerializer;
};
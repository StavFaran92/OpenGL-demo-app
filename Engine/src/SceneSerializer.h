#pragma once

class Scene;

class SceneSerializer
{
public:
	bool serialize(Scene& scene);
	bool deserialize(Scene& scene);
};
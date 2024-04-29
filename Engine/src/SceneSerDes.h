#pragma once

class Scene;

class SceneSerDes
{
public:
	bool serialize(Scene& scene);
	bool deserialize(Scene& scene);
};
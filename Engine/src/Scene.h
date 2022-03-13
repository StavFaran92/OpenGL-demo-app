#pragma once

#include <vector>
#include <memory>

class Model;
class Skybox;
class Context;
class Object3D;

class Scene
{

public:
	// -------------------- Methods -------------------- //
	void addObject(std::shared_ptr<Object3D> object);
	void removeObject(std::shared_ptr<Object3D> object);

	void addSkybox(std::shared_ptr<Skybox> skybox);
	void removeSkybox(std::shared_ptr<Skybox> skybox);

	std::shared_ptr<Skybox> getSkybox();

private:
	// -------------------- Methods -------------------- //
	friend class Context;
	void update(float deltaTime);
	inline void SetID(uint32_t id) { m_id = id; }
	void draw();

private:
	// -------------------- Attributes -------------------- //
	std::vector<std::shared_ptr<Model>> m_models;
	uint32_t m_id = 0;

	std::shared_ptr<Skybox> m_skybox = nullptr;

};

#pragma once

#include <vector>
#include <memory>

class Model;
class Skybox;

class Scene
{

public:
	// -------------------- Methods -------------------- //
	void addModel(std::shared_ptr<Model> model);
	void removeModel(std::shared_ptr<Model> model);

	void addSkybox(std::shared_ptr<Skybox> skybox);
	void removeSkybox(std::shared_ptr<Skybox> skybox);

	std::shared_ptr<Skybox> getSkybox();

private:
	// -------------------- Methods -------------------- //
	void update(float deltaTime);
	void draw();

private:
	// -------------------- Attributes -------------------- //
	std::vector<std::shared_ptr<Model>> m_models;

};

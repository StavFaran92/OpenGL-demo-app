#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <deque>

#include "Core.h"


class Model;
class Skybox;
class Context;
class Object3D;
class PointLight;
class DirectionalLight;
class Renderer;
class ObjectSelection;
class ScreenBufferProjector;
class Shader;

class EngineAPI Scene
{

public:
	// -------------------- Methods -------------------- //
	Scene() 
	{
		init();
	}

	bool addModel(Model* model);
	bool removeModel(uint32_t id);
	bool removeModel(Model* model);

	bool addPointLight(PointLight* pLight);
	bool removePointLight(PointLight* pLight);

	bool addDirectionalLight(DirectionalLight* dLight);
	bool removeDirectionalLight(DirectionalLight* dLight);

	bool addObject(Object3D* object);
	void removeObject(Object3D* object);

	void setSkybox(Skybox* skybox);
	void removeSkybox();

	void setPostProcess(bool value);
	bool setPostProcessShader(Shader* shader);

	std::shared_ptr<Renderer> getRenderer() const;
	std::shared_ptr<Renderer> getSkyboxRenderer();

	std::shared_ptr<Skybox> getSkybox();

	uint32_t getID() const { return m_id; }

	std::shared_ptr<ObjectSelection> GetObjectSelection() const;

	void draw(Model* model);


private:
	// -------------------- Methods -------------------- //
	friend class Context;
	void update(float deltaTime);
	inline void SetID(uint32_t id) { m_id = id; }
	void draw(float deltaTime);

	void init();
	void clear();
	void close();

private:
	// -------------------- Attributes -------------------- //
	std::unordered_map<uint32_t, std::shared_ptr<Model>> m_models;
	uint32_t m_modelCounter = 0;
	std::unordered_map<uint32_t, std::shared_ptr<PointLight>> m_pointLights;
	uint32_t m_pointLightCounter = 0;
	std::unordered_map<uint32_t, std::shared_ptr<DirectionalLight>> m_directionalLights;
	uint32_t m_directionalLightCounter = 0;

	std::shared_ptr<Renderer> m_renderer = nullptr;
	std::shared_ptr<Renderer> m_skyboxRenderer = nullptr;

	uint32_t m_id = 0;


	std::shared_ptr<Skybox> m_skybox = nullptr;
	std::shared_ptr<ObjectSelection> m_objectSelection = nullptr;
	std::shared_ptr<ScreenBufferProjector> m_screenBufferProjector = nullptr;

	std::deque<Model*> m_drawQueue;

	bool m_isPostProcessEnabled = false;


};

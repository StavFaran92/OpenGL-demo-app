#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <map>
#include <functional>

#include "Core.h"

#include "InstanceBatch.h"

#include "glm/glm.hpp"


class Model;
class Skybox;
class Context;
class Object3D;
class PointLight;
class DirectionalLight;
class Renderer;
class ObjectSelection;
class PostProcessProjector;
class Shader;
class CoroutineSystem;
class Context;
class ObjectPicker;
class Transformation;
class GpuInstancingRenderer;
class Entity;
template<typename T> class ObjectHandler;

class EngineAPI Scene
{
public:
	struct Params
	{
		const Scene* scene;
	};

	struct DrawQueuePreRenderParams : public Scene::Params
	{
		const Model* model;
		std::vector<const DirectionalLight*> directionalLights;
		std::vector<const PointLight*> pointLights;
	};

	enum class RenderPhase
	{
		DRAW_QUEUE_PRE_RENDER,
		DRAW_QUEUE_POST_RENDER,
	};

	using RenderCallback = std::function<void(const Params*)>;
public:
	// -------------------- Methods -------------------- //
	Scene(Context* context);

	void drawSkybox(ObjectHandler<Skybox> skybox);

	void setPostProcess(bool value);
	bool setPostProcessShader(Shader* shader);

	void addCoroutine(const std::function<bool(float)>& coroutine);
	void removeCoroutine(std::function<bool(float)> coroutine);

	std::shared_ptr<Renderer> getRenderer() const;
	std::shared_ptr<Renderer> getSkyboxRenderer();

	Skybox* getSkybox();

	uint32_t getID() const { return m_id; }

	bool isSelected(uint32_t id) const;
	void enableObjectSelection(bool isEnabled);
	void selectObject(uint32_t id);
	void clearObjectSelection();

	//void addGUI();

	void drawMultiple(const InstanceBatch& batch);

	RenderCallback* addRenderCallback(RenderPhase renderPhase, RenderCallback renderCallback);
	void removeRenderCallback(RenderCallback* callback);

	entt::registry& getRegistry();

	std::shared_ptr<Entity> createEntity();
	void removeEntity(std::shared_ptr<Entity> e);
	


private:
	// -------------------- Methods -------------------- //
	friend class Context;
	void update(float deltaTime);
	inline void SetID(uint32_t id) { m_id = id; }
	void draw(float deltaTime);

	void init(Context* context);
	void clear();
	void close();

private:
	// -------------------- Attributes -------------------- //
	std::shared_ptr<Renderer> m_renderer = nullptr;
	std::shared_ptr<GpuInstancingRenderer> m_gpuInstancingRenderer = nullptr;
	std::shared_ptr<Renderer> m_skyboxRenderer = nullptr;

	uint32_t m_id = 0;

	std::shared_ptr<ObjectSelection> m_objectSelection = nullptr;
	std::shared_ptr<PostProcessProjector> m_postProcessProjector = nullptr;
	std::shared_ptr<CoroutineSystem> m_coroutineManager = nullptr;
	std::shared_ptr<ObjectPicker> m_objectPicker = nullptr;

	std::deque<Model*> m_drawQueue;
	std::deque<Object3D*> m_updateQueue;
	Skybox* m_skybox = nullptr;

	std::deque<Model*> m_debugModelDeque;
	std::deque<std::shared_ptr<InstanceBatch>> m_instanceBatchQueue;

	Context* m_context = nullptr;

	bool m_isPostProcessEnabled = false;
	bool m_pickingPhaseActive = false;
	bool m_isObjectSelectionEnabled = false;

	entt::registry m_registry;
	std::map<RenderPhase, std::vector<RenderCallback>> m_renderCallbacks;
};

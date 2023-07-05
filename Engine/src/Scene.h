#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <map>
#include <functional>

#include "Core.h"
#include "entt/entt.hpp"

#include "glm/glm.hpp"
#include "IRenderer.h"


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
class IRenderer;
class Mesh;
class Transformation;
class InstanceBatch;
class SkyboxRenderer;
namespace physx {
	class PxScene;
}
template<typename T> class ObjectHandler;

class EngineAPI Scene
{
public:
	enum class RenderPhase
	{
		PRE_RENDER_BEGIN,
		PRE_RENDER_END,
		DRAW_QUEUE_PRE_RENDER,
		DRAW_QUEUE_POST_RENDER,
		POST_RENDER_BEGIN,
		POST_RENDER_END
	};

	using RenderCallback = std::function<void(const IRenderer::Params*)>;
public:
	// -------------------- Methods -------------------- //
	Scene(Context* context);

	void setPostProcess(bool value);
	bool setPostProcessShader(Shader* shader);

	void addCoroutine(const std::function<bool(float)>& coroutine);
	void removeCoroutine(std::function<bool(float)> coroutine);

	std::shared_ptr<Renderer> getRenderer() const;

	uint32_t getID() const { return m_id; }

	bool isSelected(uint32_t id) const;
	bool isObjectSelectionEnabled() const;
	void enableObjectSelection(bool isEnabled);
	void selectObject(uint32_t id);
	void clearObjectSelection();
	bool isPickingPhaseActive() const;

	//void addGUI();

	RenderCallback* addRenderCallback(RenderPhase renderPhase, RenderCallback renderCallback);
	void removeRenderCallback(RenderCallback* callback);

	entt::registry& getRegistry();

	Entity createEntity();
	void removeEntity(const Entity& e);
	


private:
	// -------------------- Methods -------------------- //
	friend class Context;
	void update(float deltaTime);
	inline void SetID(uint32_t id) { m_id = id; }
	void draw(float deltaTime);

	void startSimulation();
	void stopSimulation();
	bool isSimulationActive() const;

	void init(Context* context);
	void clear();
	void close();

private:
	// -------------------- Attributes -------------------- //
	uint32_t m_id = 0;
	Context* m_context = nullptr;

	// Renderers
	std::shared_ptr<Renderer> m_renderer = nullptr;
	std::shared_ptr<GpuInstancingRenderer> m_gpuInstancingRenderer = nullptr;
	std::shared_ptr<SkyboxRenderer> m_skyboxRenderer = nullptr;

	// Scene Services
	std::shared_ptr<ObjectSelection> m_objectSelection = nullptr;
	std::shared_ptr<PostProcessProjector> m_postProcessProjector = nullptr;
	std::shared_ptr<CoroutineSystem> m_coroutineManager = nullptr;
	std::shared_ptr<ObjectPicker> m_objectPicker = nullptr;

	entt::registry m_registry;
	std::map<RenderPhase, std::vector<RenderCallback>> m_renderCallbacks;

	physx::PxScene* m_PhysicsScene = nullptr;

	bool m_isSimulationActive = false;

	Mesh* m_tempBoxMesh = nullptr;
	Shader* m_tempOutlineShader = nullptr;
};

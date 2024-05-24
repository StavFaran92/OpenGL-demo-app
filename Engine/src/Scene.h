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
class ICamera;
class PhysicsSystem;
class ShadowSystem;
class UniformBufferObject;
class LightSystem;
class DeferredRenderer;
class FrameBufferObject;
class RenderBufferObject;
class TextureHandler;
struct RigidBodyComponent;
namespace physx {
	class PxScene;
	class PxShape;
	class PxRigidActor;
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

	using RenderCallback = std::function<void(const IRenderer::DrawQueueRenderParams*)>;
public:
	// -------------------- Methods -------------------- //
	Scene(Context* context);

	void setPostProcess(bool value);
	bool setPostProcessShader(Shader* shader);

	void addCoroutine(const std::function<bool(float)>& coroutine);
	void removeCoroutine(std::function<bool(float)> coroutine);

	std::shared_ptr<IRenderer> getRenderer() const;

	uint32_t getID() const { return m_id; }

	bool isObjectSelectionEnabled() const;
	void enableObjectSelection(bool isEnabled);
	void selectObject(uint32_t id);
	void clearObjectSelection();
	bool isPickingPhaseActive() const;

	RenderCallback* addRenderCallback(RenderPhase renderPhase, RenderCallback renderCallback);
	void removeRenderCallback(RenderCallback* callback);

	entt::registry& getRegistry();
	const entt::registry& getRegistry() const;

	Entity createEntity();
	Entity createEntity(const std::string& name);
	void removeEntity(const Entity& e);

	ICamera* getActiveCamera() const;
	
	void displayWireframeMesh(Entity e, IRenderer::DrawQueueRenderParams params);

	void setIBLData(Resource<Texture> irradianceMap, Resource<Texture> prefilterEnvMap);

	int getRenderTarget() const;

	//void setPrimaryCamera(ICamera* camera);

private:
	// -------------------- Methods -------------------- //
	friend class Context;
	friend class SceneSerializer;
	void update(float deltaTime);
	inline void SetID(uint32_t id) { m_id = id; }
	void draw(float deltaTime);

	void startSimulation();
	void createSimulationActors(PhysicsSystem* physicsSystem);
	void createActor(entt::entity entity, PhysicsSystem* physicsSystem, RigidBodyComponent& rb);
	void removeActor(entt::entity entity, PhysicsSystem* physicsSystem, RigidBodyComponent& rb);
	void stopSimulation();
	bool isSimulationActive() const;
	void createShape(PhysicsSystem* physicsSystem, physx::PxRigidActor* body, Entity e, bool recursive);

	void onRigidBodyConstruct(entt::registry& registry, entt::entity entity);
	void onRigidBodyDestroy(entt::registry& registry, entt::entity entity);

	void onCollisionConstruct(entt::registry& registry, entt::entity entity);

	void init(Context* context);
	void clear();
	void close();

private:
	// -------------------- Attributes -------------------- //
	uint32_t m_id = 0;
	Context* m_context = nullptr;

	// Renderers
	std::shared_ptr<DeferredRenderer> m_deferredRenderer = nullptr;
	std::shared_ptr<Renderer> m_forwardRenderer = nullptr;
	//std::shared_ptr<GpuInstancingRenderer> m_gpuInstancingRenderer = nullptr;
	//std::shared_ptr<SkyboxRenderer> m_skyboxRenderer = nullptr;

	// Scene Services
	std::shared_ptr<ObjectSelection> m_objectSelection = nullptr;
	std::shared_ptr<PostProcessProjector> m_postProcessProjector = nullptr;
	std::shared_ptr<CoroutineSystem> m_coroutineManager = nullptr;
	std::shared_ptr<ObjectPicker> m_objectPicker = nullptr;
	std::shared_ptr<ShadowSystem> m_shadowSystem;
	std::shared_ptr<LightSystem> m_lightSystem;

	entt::registry m_registry;
	std::map<RenderPhase, std::vector<RenderCallback>> m_renderCallbacks;

	physx::PxScene* m_PhysicsScene = nullptr;

	bool m_isSimulationActive = false;

	Shader* m_tempOutlineShader = nullptr;

	//CameraComponent* m_activeCamera = nullptr;

	glm::mat4 m_defaultPerspectiveProjection;

	std::shared_ptr<UniformBufferObject> m_uboTime;

	std::shared_ptr<FrameBufferObject> m_renderTargetFBO;
	std::shared_ptr<RenderBufferObject> m_renderTargetRBO;
	Resource<Texture> m_renderTargetTexture;

	Resource<Texture> m_irradianceMap;
	Resource<Texture> m_prefilterEnvMap;
	Resource<Texture> m_BRDFIntegrationLUT;
	std::shared_ptr<Shader> m_skyboxShader;
};

#include "Scene.h"

#include "Skybox.h"
#include "Object3D.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Engine.h"
#include "ICamera.h"
#include "ObjectSelection.h"
#include "SkyboxRenderer.h"
#include "PostProcessProjector.h"
#include "CoroutineSystem.h"
#include "Logger.h"
#include "PhongShader.h"
#include "PickingShader.h"
#include "Context.h"
#include "Window.h"
#include "ObjectPicker.h"
#include "ObjectManager.h"
#include "ObjectHandler.h"
#include "Configurations.h"
#include "GpuInstancingRenderer.h"
#include "InstanceBatch.h"
#include "StandardShader.h"
#include "Entity.h"
#include "Transformation.h"
#include "Mesh.h"
#include "RenderableComponent.h"
#include "Component.h"
#include "Shader.h"
#include "InstanceBatch.h"
#include "SkyboxRenderer.h"
#include "Material.h"
#include "DefaultMaterial.h"
#include "ScriptableEntity.h"
#include "PhysicsSystem.h"
#include "PhysXUtils.h"

void Scene::init(Context* context)
{
	m_context = context;

	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	m_renderer = std::make_shared<Renderer>();
	m_skyboxRenderer = std::make_shared<SkyboxRenderer>(*m_renderer.get());
	m_gpuInstancingRenderer = std::make_shared<GpuInstancingRenderer>();
	m_objectSelection = std::make_shared<ObjectSelection>(m_context, this);
	m_objectPicker = std::make_shared<ObjectPicker>(m_context, this);
	if (!m_objectPicker->init(width, height))
	{
		logError("Object picker failed to init!");
	}

	m_postProcessProjector = std::make_shared<PostProcessProjector>(this);
	if (!m_postProcessProjector->init(width, height))
	{
		logError("Post process projector failed to init!");
	}
	m_postProcessProjector->setEnabled(true);

	m_coroutineManager = std::make_shared<CoroutineSystem>();

	m_PhysicsScene = Engine::get()->getPhysicsSystem()->createScene();

#ifdef SGE_DEBUG
	physx::PxPvdSceneClient* pvdClient = m_PhysicsScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif // SGE_DEBUG

	// Add default dir light
	createEntity().addComponent<DirectionalLight>();
}

void Scene::update(float deltaTime)
{
	m_renderer->getCamera()->update(deltaTime);

	// Advance all coroutines
	auto coroutines = m_coroutineManager->getAllCoroutines();
	for (int i = 0; i < coroutines.size(); i++)
	{
		if ((*coroutines[i])(deltaTime))
		{
			m_coroutineManager->removeCoroutine(i);
		}
	}

	// Run all User Scriptable Entities scripts
	for (auto&& [entity, nsc] : m_registry.view<NativeScriptComponent>().each())
	{
		if (!nsc.instantiateScript)
		{
			logWarning("Native Script does not have an instantiation method, did you forget to call Bind()?");
			continue;
		}

		if (!nsc.script)
		{
			nsc.script = nsc.instantiateScript();
			assert(nsc.script);
			nsc.script->entity = Entity{entity, this};
			nsc.script->onCreate();
		}

		nsc.script->onUpdate(deltaTime);
	}

	// Physics
	m_PhysicsScene->simulate(1/180.f);
	m_PhysicsScene->fetchResults(true);

	//physx::PxU32 nbActors = m_PhysicsScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	//if (nbActors)
	//{
	//	std::vector<physx::PxRigidActor*> actors(nbActors);
	//	m_PhysicsScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<physx::PxActor**>(&actors[0]), nbActors);

	//	for (physx::PxRigidActor* actor : actors)
	//	{
	//		entity_id id = (entity_id)actor->userData;
	//		Entity e{ (entt::entity)id, this };
	//		auto& rb = e.getComponent<RigidBodyComponent>();
	//		auto& transform = e.getComponent<Transformation>();

	//		physx::PxTransform pxTransform = actor->getGlobalPose();
	//		transform.setPosition({ pxTransform.p.x, pxTransform.p.y, pxTransform.p.z });
	//	}
	//}

	for (auto&& [entity, rb] : m_registry.view<RigidBodyComponent>().each())
	{
		Entity e{ entity, this };
		auto& transform = e.getComponent<Transformation>();
		physx::PxRigidActor* actor = (physx::PxRigidActor*)rb.simulatedBody;
		
		physx::PxTransform pxTransform = actor->getGlobalPose();
		PhysXUtils::fromPhysXTransform(e, pxTransform, transform);
	}

	for (auto&& [entity, transformation] : m_registry.view<Transformation>().each())
	{
		transformation.update(deltaTime);
	}
}

void Scene::draw(float deltaTime)
{
	m_renderer->clear();

	IRenderer::DrawQueueRenderParams params;
	params.scene = this;
	params.context = m_context;
	params.registry = &m_registry;
	params.renderer = m_renderer.get();

	// PRE Render Phase
	for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_BEGIN])
	{
		cb(&params);
	}

	for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_END])
	{
		cb(&params);
	}

	// Render Phase
	for (auto&& [entity, mesh, transform, renderable] : 
		m_registry.view<Mesh, Transformation, RenderableComponent>().each())
	{
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = &mesh;
		params.transform = &transform;
			
		for (const auto& cb : m_renderCallbacks[RenderPhase::DRAW_QUEUE_PRE_RENDER])
		{
			cb(&params);
		}

		auto& shader = entityhandler.getComponentInParent<StandardShader>();
		shader.updateDirLights(m_registry);
		shader.updatePointLights(m_registry);
		shader.setViewPos(m_renderer->getCamera()->getPosition());

		// draw model
		m_renderer->render(params);

		for (const auto& cb : m_renderCallbacks[RenderPhase::DRAW_QUEUE_POST_RENDER])
		{
			cb(&params);
		}

		params.entity = nullptr;
		params.mesh = nullptr;
		params.transform = nullptr;
	};

	// POST Render Phase
	// Iterate GPU instancing batches
	for (auto&& [entity, mesh, instanceBatch] : m_registry.view<Mesh, InstanceBatch>().each())
	{
		// draw model
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = &mesh;
		m_gpuInstancingRenderer->render(params);
	}

	// For some reason this group destroys the entities
	for (auto&& [entity, skybox, mesh, transform, mat, shader] : 
		m_registry.view<SkyboxComponent, Mesh, Transformation, DefaultMaterial, StandardShader>().each())
	{
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = &mesh;
		params.transform = &transform;
		params.shader = &shader;

		m_skyboxRenderer->render(params);

		params.entity = nullptr;
		params.mesh = nullptr;
		params.transform = nullptr;
		params.shader = nullptr;
	}

	//if (DEBUG_MODE_ENABLED && DEBUG_DISPLAY_NORMALS)
	//{
	//	// Use normal display shader
	//	auto normalDisplayShader = m_context->getNormalDisplayShader();

	//	// draw scene
	//	while (!m_debugModelDeque.empty())
	//	{
	//		auto model = m_debugModelDeque.front();
	//		m_debugModelDeque.pop_front();

	//		m_renderer->render(model, normalDisplayShader);
	//	}
	//}

#ifdef SGE_DEBUG
	for (auto&& [entity, rb] : m_registry.view<RigidBodyComponent>().each())
	{
		Entity e{ entity, this };
		physx::PxRigidActor* actor = (physx::PxRigidActor*)rb.simulatedBody;
		physx::PxShape* shape = nullptr;
		actor->getShapes(&shape, 1);
		physx::PxGeometry geom = shape->getGeometry().any();
	}
#endif //SGE_DEBUG

	for (const auto& cb : m_renderCallbacks[RenderPhase::POST_RENDER_BEGIN])
	{
		cb(&params);
	}

	for (const auto& cb : m_renderCallbacks[RenderPhase::POST_RENDER_END])
	{
		cb(&params);
	}
}

Scene::RenderCallback* Scene::addRenderCallback(RenderPhase renderPhase, RenderCallback renderCallback)
{
	if (!renderCallback)
	{
		logError("render callback cannot be null.");
		return 0;
	}

	m_renderCallbacks[renderPhase].push_back(std::move(renderCallback));
	return &m_renderCallbacks[renderPhase].back();
}

void Scene::removeRenderCallback(RenderCallback* callback)
{
	for (auto& pair : m_renderCallbacks) {
		auto& vector = pair.second;
		vector.erase(
			std::remove_if(vector.begin(), vector.end(),
				[callback](const auto& existingCallback) { return &existingCallback == callback; }),
			vector.end()
		);
	}
}

entt::registry& Scene::getRegistry()
{
	return m_registry;
}

Entity Scene::createEntity()
{
	entt::entity e = m_registry.create();
	auto entityHandler = Entity(e, this);
	entityHandler.addComponent<Transformation>(entityHandler);
	entityHandler.addComponent<HierarchyComponent>();
	return entityHandler;
}

void Scene::removeEntity(const Entity& e)
{
	m_registry.destroy(e.handler());
}


void Scene::clear()
{
	m_registry.clear();
}

Scene::Scene(Context* context)
{
	init(context);
}

std::shared_ptr<Renderer> Scene::getRenderer() const
{
	return m_renderer;
}

void Scene::close()
{
	clear();
}

void Scene::setPostProcess(bool value)
{
	m_postProcessProjector->setEnabled(value);
}

//bool Scene::isSelected(uint32_t id) const
//{
//	if (!m_isObjectSelectionEnabled)
//	{
//		logWarning("Object selection isn't enabled for this scene.");
//		return false;
//	}
//
//	return m_objectSelection->isObjectSelected(id);
//}

bool Scene::isObjectSelectionEnabled() const
{
	return m_objectSelection->isEnabled();
}

void Scene::enableObjectSelection(bool isEnabled)
{
	m_objectSelection->enableObjectSelection(isEnabled);
}

void Scene::selectObject(uint32_t id)
{
	m_objectSelection->selectObject(id);
}

void Scene::clearObjectSelection()
{
	m_objectSelection->clear();
}

bool Scene::setPostProcessShader(Shader* shader)
{
	if (m_postProcessProjector)
	{
		m_postProcessProjector->setPostProcessShader(std::shared_ptr<Shader>(shader));
		return true;
	}
	return false;
}

void Scene::addCoroutine(const std::function<bool(float)>& coroutine)
{
	m_coroutineManager->addCoroutine(coroutine);
}

//void Scene::removeCoroutine(std::function<bool(float)>* coroutine)
//{
//	m_coroutineManager->removeCoroutine(coroutine);
//}

bool Scene::isPickingPhaseActive() const
{
	return m_objectPicker->isPickingPhaseActive();
}

void Scene::startSimulation()
{
	if (m_isSimulationActive)
	{
		logWarning("Simulation already active.");
		return;
	}

	for (auto&& [entity, rb] : m_registry.view<RigidBodyComponent>().each())
	{
		Entity e{ entity, this };
		auto physicsSystem = Engine::get()->getPhysicsSystem();

		auto& transform = e.getComponent<Transformation>();
		auto scale = transform.getScale();

 		auto body = physicsSystem->createRigidBody(transform, rb.type);

		if (e.HasComponent<CollisionBoxComponent>())
		{
			auto& collider = e.getComponent<CollisionBoxComponent>();
			
			physx::PxShape* shape = physicsSystem->createBoxShape(collider.halfExtent * scale.x, collider.halfExtent * scale.y, collider.halfExtent * scale.z);
			body->attachShape(*shape);
			shape->release();
		}

		assert(body);

		m_PhysicsScene->addActor(*body);
		rb.simulatedBody = body;
	}

	m_isSimulationActive = true;
}

void Scene::stopSimulation()
{
	if (!m_isSimulationActive)
	{
		logWarning("Simulation already stopped.");
		return;
	}

	m_isSimulationActive = false;
}

bool Scene::isSimulationActive() const
{
	return m_isSimulationActive;
}

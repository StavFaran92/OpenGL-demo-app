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
#include "Shader.h"
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
#include "Box.h"
#include "EditorCamera.h"
#include "ShadowSystem.h"
#include "Engine.h"
#include "TimeManager.h"
#include <GL/glew.h>

void Scene::displayWireframeMesh(Entity e, IRenderer::DrawQueueRenderParams params)
{
	auto mesh = e.tryGetComponent<MeshComponent>();

	if (mesh)
	{
		params.entity = &e;
		params.shader = m_tempOutlineShader;
		params.mesh = mesh->mesh.get();
		auto tempModel = e.getComponent<Transformation>().getWorldTransformation();
		params.model = &tempModel;

		m_renderer->render(params);

		params.entity = nullptr;
		params.shader = nullptr;
		params.mesh = nullptr;
		params.model = nullptr;
	}

	auto children = e.getChildren();
	if (children.size() > 0)
	{
		for (auto& [e, childEnt] : children)
		{
			displayWireframeMesh(childEnt, params);
		}
	}
}

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
	m_postProcessProjector->setEnabled(false);

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

	m_PhysicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
	m_PhysicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

	m_shadowSystem = std::make_shared<ShadowSystem>(m_context, this);
	if (!m_shadowSystem->init())
	{
		logError("Shadow System init failed!");
	} 

	m_defaultPerspectiveProjection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);
	
#endif // SGE_DEBUG

	// Add default dir light
	createEntity().addComponent<DirectionalLight>();

	auto editorCamera = createEntity();
	editorCamera.addComponent<CameraComponent>();
	m_activeCamera = editorCamera.addComponent<NativeScriptComponent>().bind<EditorCamera>();

	m_tempBoxMesh = Box::createMesh();
	m_tempOutlineShader = Shader::create<Shader>("Resources/Engine/Shaders/shader.vert", "Resources/Engine/Shaders/OutlineShader.frag");

	m_registry.on_construct<RigidBodyComponent>().connect<&Scene::onRigidBodyConstruct>(this);
	m_registry.on_construct<CollisionBoxComponent>().connect<&Scene::onCollisionConstruct>(this);
	m_registry.on_construct<CollisionSphereComponent>().connect<&Scene::onCollisionConstruct>(this);
	m_registry.on_construct<CollisionMeshComponent>().connect<&Scene::onCollisionConstruct>(this);
}

void Scene::update(float deltaTime)
{
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
		if (!nsc.script)
		{
			logWarning("Native Script cannot be Null, did you forget to call Bind()?");
			continue;
		}

		nsc.script->onUpdate(deltaTime);
	}

	// Physics
	m_PhysicsScene->simulate(1/120.f);
	m_PhysicsScene->fetchResults(true);

	physx::PxU32 nbActors = m_PhysicsScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<physx::PxRigidActor*> actors(nbActors);
		m_PhysicsScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<physx::PxActor**>(&actors[0]), nbActors);

		for (physx::PxRigidActor* actor : actors)
		{
			entity_id id = *(entity_id*)actor->userData;
			Entity e{ entt::entity(id), this };
			auto& transform = e.getComponent<Transformation>();

			physx::PxTransform pxTransform = actor->getGlobalPose();
			PhysXUtils::fromPhysXTransform(e, pxTransform, transform);
		}
	}

	//for (auto&& [entity, rb] : m_registry.view<RigidBodyComponent>().each())
	//{
	//	Entity e{ entity, this };
	//	auto& transform = e.getComponent<Transformation>();
	//	physx::PxRigidActor* actor = (physx::PxRigidActor*)rb.simulatedBody;
	//	
	//	physx::PxTransform pxTransform = actor->getGlobalPose();
	//	PhysXUtils::fromPhysXTransform(e, pxTransform, transform);
	//}

	//for (auto&& [entity, transformation] : m_registry.view<Transformation>().each())
	//{
	//	transformation.update(deltaTime);
	//}
}

void Scene::draw(float deltaTime)
{
	m_renderer->clear();

	IRenderer::DrawQueueRenderParams params;
	params.scene = this;
	params.context = m_context;
	params.registry = &m_registry;
	params.renderer = m_renderer.get();
	auto tempView = m_activeCamera->getView();
	params.view = &tempView;
	params.projection = &m_defaultPerspectiveProjection;

	// PRE Render Phase
	for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_BEGIN])
	{
		cb(&params);
	}

	for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_END])
	{
		cb(&params);
	}

	auto phongShader = m_context->getStandardShader();
	PhongShader::updateDirLights(phongShader, m_registry);
	PhongShader::updatePointLights(phongShader, m_registry);

	// Set time elapsed
	auto elapsed = (float)Engine::get()->getTimeManager()->getElapsedTime(TimeManager::Duration::MilliSeconds) / 1000;
	phongShader->setTime(elapsed);

	// Render Phase
	for (auto&& [entity, mesh, transform, renderable] : 
		m_registry.view<MeshComponent, Transformation, RenderableComponent>().each())
	{
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = mesh.mesh.get();
		auto tempModel = transform.getWorldTransformation();
		params.model = &tempModel;
			
		for (const auto& cb : m_renderCallbacks[RenderPhase::DRAW_QUEUE_PRE_RENDER])
		{
			cb(&params);
		}

		// draw model
		m_renderer->render(params);

		for (const auto& cb : m_renderCallbacks[RenderPhase::DRAW_QUEUE_POST_RENDER])
		{
			cb(&params);
		}

		params.entity = nullptr;
		params.mesh = nullptr;
		params.model = nullptr;
	};

	// POST Render Phase
	// Iterate GPU instancing batches
	for (auto&& [entity, mesh, instanceBatch] : m_registry.view<MeshComponent, InstanceBatch>().each())
	{
		// draw model
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = mesh.mesh.get();
		m_gpuInstancingRenderer->render(params);
	}

	// For some reason this group destroys the entities
	for (auto&& [entity, skybox, mesh, transform, mat, shader] : 
		m_registry.view<SkyboxComponent, MeshComponent, Transformation, DefaultMaterial, Shader>().each())
	{
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = mesh.mesh.get();
		auto tempModel = transform.getWorldTransformation();
		params.model = &tempModel;
		params.shader = &shader;

		m_skyboxRenderer->render(params);

		params.entity = nullptr;
		params.mesh = nullptr;
		params.shader = nullptr;
		params.model = nullptr;
	}

#if 0
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.0, -1.0);
	for (auto&& [entity, cb] : m_registry.view<CollisionBoxComponent>().each())
	{
		Entity e{ entity, this };

		displayWireframeMesh(e, params);
	}

	for (auto&& [entity, cb] : m_registry.view<CollisionSphereComponent>().each())
	{
		Entity e{ entity, this };

		displayWireframeMesh(e, params);
	}

	for (auto&& [entity, cb] : m_registry.view<CollisionMeshComponent>().each())
	{
		Entity e{ entity, this };

		displayWireframeMesh(e, params);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);
#endif

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
	return entityHandler;
}

void Scene::removeEntity(const Entity& e)
{
	m_registry.destroy(e.handler());
}

ICamera* Scene::getActiveCamera() const
{
	return m_activeCamera;
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

	auto physicsSystem = Engine::get()->getPhysicsSystem();

	createSimulationActors(physicsSystem);

	//for (auto&& [entity, rb] : m_registry.view<RigidBodyComponent>().each())
	//{
	//	Entity e{ entity, this };
	//	for (auto [ec, child] : e.getChildren())
	//	{
	//		if (child.valid())
	//		{
	//			auto childRigidBody = child.tryGetComponent<RigidBodyComponent>();
	//			if (!childRigidBody)
	//			{
	//				continue;
	//			}
	//			auto& transform = e.getComponent<Transformation>();
	//			auto& parentTransform = child.getComponent<Transformation>();
	//			

	//			auto joint = physx::PxFixedJointCreate(*physicsSystem->getPhysics(), (physx::PxRigidActor*)rb.simulatedBody, PhysXUtils::toPhysXTransform(parentTransform), (physx::PxRigidActor*)childRigidBody->simulatedBody, PhysXUtils::toPhysXTransform(transform));
	//			joint->setConstraintFlags(physx::PxConstraintFlag::eVISUALIZATION);
	//		}
	//	}
	//}

	//for (auto&& [entity, rb] : m_registry.view<RigidBodyComponent>().each())
	//{
	//	Entity e{ entity, this };

	//	auto& transform = e.getComponent<Transformation>();
	//	auto scale = transform.getScale();
 //		auto body = physicsSystem->createRigidBody(transform, rb.type, rb.mass);

	//	if (e.HasComponent<CollisionBoxComponent>())
	//	{
	//		auto& collider = e.getComponent<CollisionBoxComponent>();
	//		physx::PxShape* shape = physicsSystem->createBoxShape(collider.halfExtent * scale.x, collider.halfExtent * scale.y, collider.halfExtent * scale.z);
	//		body->attachShape(*shape);
	//		shape->release();
	//	}
	//	else if (e.HasComponent<CollisionSphereComponent>())
	//	{
	//		auto& collider = e.getComponent<CollisionSphereComponent>();
	//		physx::PxShape* shape = physicsSystem->createSphereShape(collider.radius * std::max(std::max(scale.x, scale.y), scale.z));
	//		body->attachShape(*shape);
	//		shape->release();
	//	}
	//	else if (e.HasComponent<CollisionConvexMeshComponent>())
	//	{
	//		//auto& mesh = e.getComponent<Mesh>();
	//		std::vector<glm::vec3> pos = { {0,0,0}, {0,1,0}, {1,0,0} };
	//		physx::PxShape* shape = physicsSystem->createConvexMeshShape(pos);
	//		//physx::PxShape* shape = physicsSystem->createConvexMeshShape(*mesh.getPositions());
	//		body->attachShape(*shape);
	//		shape->release();
	//	}


	//	assert(body);

	//	m_PhysicsScene->addActor(*body);
	//	rb.simulatedBody = body;
	//}

	m_isSimulationActive = true;
}

void Scene::createSimulationActors(PhysicsSystem* physicsSystem)
{
	for (auto&& [entity, rb] : m_registry.view<RigidBodyComponent>().each())
	{
		createActor(entity, physicsSystem, rb);
	}
}

void Scene::createActor(entt::entity entity, PhysicsSystem* physicsSystem, RigidBodyComponent& rb)
{
	Entity e{ entity, this };

	auto& transform = e.getComponent<Transformation>();
	auto body = physicsSystem->createRigidBody(transform, rb.type, rb.mass);

	createShape(physicsSystem, body, e, true);

	m_PhysicsScene->addActor(*body);
	entity_id* id = new entity_id(e.handlerID());
	body->userData = (void*)id;
	rb.simulatedBody = (void*)body;
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

void Scene::createShape(PhysicsSystem* physicsSystem, physx::PxRigidActor* body, Entity e, bool recursive)
{
	physx::PxShape* shape = nullptr;
	auto& transform = e.getComponent<Transformation>();
	auto scale = transform.getWorldScale();

	if (e.HasComponent<CollisionBoxComponent>())
	{
		auto& collider = e.getComponent<CollisionBoxComponent>();
		shape = physicsSystem->createBoxShape(collider.halfExtent * scale.x, collider.halfExtent * scale.y, collider.halfExtent * scale.z);
	}
	else if (e.HasComponent<CollisionSphereComponent>())
	{
		auto& collider = e.getComponent<CollisionSphereComponent>();
		shape = physicsSystem->createSphereShape(collider.radius * std::max(std::max(scale.x, scale.y), scale.z));
	}
	else if (e.HasComponent<CollisionMeshComponent>())
	{
		auto meshComponent = e.tryGetComponent<MeshComponent>();
		if (meshComponent)
		{
			const std::vector<glm::vec3>* apos = meshComponent->mesh->getPositions();
			shape = physicsSystem->createConvexMeshShape(*apos);
		}
	}

	if (shape)
	{
		
		auto translation = transform.getWorldPosition();
		auto orientation = transform.getWorldRotation();

		physx::PxVec3 pxTranslation(translation.x, translation.y, translation.z);
		pxTranslation -= body->getGlobalPose().p;
		physx::PxQuat pxRotation(orientation.x, orientation.y, orientation.z, orientation.w);
		pxRotation *= body->getGlobalPose().q.getConjugate();

		auto physxTransform = physx::PxTransform(pxTranslation, pxRotation);

		//auto physxTransform = PhysXUtils::toPhysXTransform(transform);
		shape->setLocalPose(physxTransform);
		body->attachShape(*shape);
		shape->release();
	}

	if (recursive)
	{
		for (auto [eid, child] : e.getChildren())
		{
			createShape(physicsSystem, body, child, true);
		}
	}
}

void Scene::onRigidBodyConstruct(entt::registry& registry, entt::entity entity)
{
	// if add rigid body
		// create actor
			// create shape recursive
	// if add collision
		// look for rigid body in hierarchy
			// if found add shape to body (not recursive)

	if (m_isSimulationActive)
	{
		Entity e(entity, this);
		auto physicsSystem = Engine::get()->getPhysicsSystem();
		auto& rb = e.getComponent<RigidBodyComponent>();
		createActor(entity, physicsSystem, rb);
	}
}

void Scene::onCollisionConstruct(entt::registry& registry, entt::entity entity)
{
	if (m_isSimulationActive)
	{
		Entity e(entity, this);
		auto rb = e.tryGetComponentInParent<RigidBodyComponent>(true);
		if (rb)
		{
			auto physicsSystem = Engine::get()->getPhysicsSystem();
			auto body = (physx::PxRigidActor*)rb->simulatedBody;
			createShape(physicsSystem, body, e, false);
		}
	}
}

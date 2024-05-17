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
#include "ScriptableEntity.h"
#include "PhysicsSystem.h"
#include "PhysXUtils.h"
#include "Box.h"
#include "EditorCamera.h"
#include "ShadowSystem.h"
#include "LightSystem.h"
#include "Engine.h"
#include "TimeManager.h"
#include "UniformBufferObject.h"
#include "DeferredRenderer.h"
#include "Random.h"
#include "ShapeFactory.h"
#include <GL/glew.h>
#include "EquirectangularToCubemapConverter.h"
#include "IBL.h"


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

		m_deferredRenderer->render(params);

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

void Scene::setIBLData(Resource<Texture> irradianceMap, Resource<Texture> prefilterEnvMap)
{
	m_irradianceMap = irradianceMap;
	m_prefilterEnvMap = prefilterEnvMap;
}

int Scene::getRenderTarget() const
{
	return m_renderTargetTexture.get()->getID();
}

//void Scene::setPrimaryCamera(ICamera* camera)
//{
//	m_activeCamera = camera;
//}

bool Scene::serialize()
{
	return m_serdes.serialize(*this);
}

bool Scene::deserialize()
{
	return m_serdes.deserialize(*this);
}

void Scene::init(Context* context)
{
	m_context = context;

	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	m_renderTargetFBO = std::make_shared<FrameBufferObject>();
	m_renderTargetRBO = std::make_shared<RenderBufferObject>(
		Engine::get()->getWindow()->getWidth(), 
		Engine::get()->getWindow()->getHeight());

	m_renderTargetFBO->bind();

	// Generate Texture for Position data
	m_renderTargetTexture = Texture::createEmptyTexture(width, height);
	m_renderTargetFBO->attachTexture(m_renderTargetTexture.get()->getID(), GL_COLOR_ATTACHMENT0);

	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	// Create RBO and attach to FBO
	m_renderTargetFBO->attachRenderBuffer(m_renderTargetRBO->GetID(), FrameBufferObject::AttachmentType::Depth_Stencil);

	if (!m_renderTargetFBO->isComplete())
	{
		logError("FBO is not complete!");
		return;
	}

	m_renderTargetFBO->unbind();

	m_deferredRenderer = std::make_shared<DeferredRenderer>(m_renderTargetFBO, this);
	m_deferredRenderer->init();

	m_forwardRenderer = std::make_shared<Renderer>(m_renderTargetFBO, this);
	m_forwardRenderer->init();

	//m_skyboxRenderer = std::make_shared<SkyboxRenderer>(*m_renderer.get());
	//m_gpuInstancingRenderer = std::make_shared<GpuInstancingRenderer>();
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
#endif // SGE_DEBUG

	m_shadowSystem = std::make_shared<ShadowSystem>(m_context, this);
	if (!m_shadowSystem->init())
	{
		logError("Shadow System init failed!");
	} 


	m_lightSystem = std::make_shared<LightSystem>(m_context, this);
	if (!m_lightSystem->init())
	{
		logError("Light System init failed!");
	}

	m_defaultPerspectiveProjection = glm::perspective(45.0f, (float)4 / 3, 0.1f, 100.0f);
	


	
	//dLight.getComponent<Transformation>().setLocalPosition({ 0 , 2, 0 });;
	

	//for (int i = 0; i < 32; i++)
	//{
	//	auto rand_x = Engine::get()->getRandomSystem()->rand() * 12 - 6;
	//	auto rand_y = Engine::get()->getRandomSystem()->rand() * 12 - 6;
	//	auto pLight = createEntity();
	//	//auto pLight = ShapeFactory::createSphere(this);
	//	pLight.addComponent<Material>();
	//	pLight.addComponent<RenderableComponent>();
	//	pLight.addComponent<PointLight>();
	//	pLight.getComponent<Transformation>().setLocalPosition({ rand_x , 0, rand_y });
	//	pLight.getComponent<RenderableComponent>().renderTechnique = RenderableComponent::Forward;
	//}

	m_tempOutlineShader = Shader::create<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/OutlineShader.glsl");

	m_uboTime = std::make_shared<UniformBufferObject>(sizeof(float));
	m_uboTime->attachToBindPoint(0);

	// Create irradiance map using created cubemap
	m_irradianceMap = context->getDummyTexture();

	// Create prefilter env map using created cubemap
	m_prefilterEnvMap = context->getDummyTexture();

	// Create BRDF look up texture
	m_BRDFIntegrationLUT = IBL::generateBRDFIntegrationLUT(this);

	m_skyboxShader = Shader::createShared<Shader>(SGE_ROOT_DIR +"Resources/Engine/Shaders/SkyboxShader.glsl");

	m_registry.on_construct<RigidBodyComponent>().connect<&Scene::onRigidBodyConstruct>(this);
	m_registry.on_construct<CollisionBoxComponent>().connect<&Scene::onCollisionConstruct>(this);
	m_registry.on_construct<CollisionSphereComponent>().connect<&Scene::onCollisionConstruct>(this);
	m_registry.on_construct<CollisionMeshComponent>().connect<&Scene::onCollisionConstruct>(this);
	
	m_registry.on_destroy<RigidBodyComponent>().connect<&Scene::onRigidBodyDestroy>(this);
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
	glViewport(0, 0, Engine::get()->getWindow()->getWidth(), Engine::get()->getWindow()->getHeight());
	m_deferredRenderer->clear();

	CameraComponent* activeCamera = nullptr;
	for (auto&& [entity, camera] : m_registry.view<CameraComponent>().each())
	{
		if (camera.isPrimary)
		{
			activeCamera = &camera;
			break;
		}
	}

	IRenderer::DrawQueueRenderParams params;
	params.scene = this;
	params.context = m_context;
	params.registry = &m_registry;
	params.renderer = m_forwardRenderer.get();
	auto tempView = activeCamera->getView();
	params.view = &tempView;
	params.projection = &m_defaultPerspectiveProjection;
	params.cameraPos = activeCamera->getPosition();
	params.irradianceMap = m_irradianceMap;
	params.prefilterEnvMap = m_prefilterEnvMap;
	params.brdfLUT = m_BRDFIntegrationLUT;

	m_shadowSystem->renderToDepthMap(&params);

	params.lightSpaceMatrix = m_shadowSystem->getLightSpaceMat();
	params.shadowMap = m_shadowSystem->getShadowMap();

	// PRE Render Phase
	for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_BEGIN])
	{
		cb(&params);
	}

	for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_END])
	{
		cb(&params);
	}

	// Set time elapsed
	auto elapsed = (float)Engine::get()->getTimeManager()->getElapsedTime(TimeManager::Duration::MilliSeconds) / 1000;

	m_uboTime->bind();
	m_uboTime->setData(0, sizeof(float), &elapsed);
	m_uboTime->unbind();


	auto view = m_registry.view<MeshComponent, Transformation, RenderableComponent>();

	// TODO consider optimizing this
	std::vector<Entity> deferredRendererEntityGroup;
	std::vector<Entity> forwardRendererEntityGroup;
	auto iter = view.begin();
	while (iter != view.end())
	{
		Entity entityhandler{ *iter, this };

		auto& renderable = entityhandler.getComponent<RenderableComponent>();
		if (renderable.renderTechnique == RenderableComponent::RenderTechnique::Deferred)
		{
			deferredRendererEntityGroup.push_back(entityhandler);
		}
		else if (renderable.renderTechnique == RenderableComponent::RenderTechnique::Forward)
		{
			forwardRendererEntityGroup.push_back(entityhandler);
		}

		iter++;
	}

	params.entityGroup = &deferredRendererEntityGroup;

	m_deferredRenderer->renderScene(params);

	const FrameBufferObject& gBuffer = m_deferredRenderer->getGBuffer();
	auto rTarget = m_forwardRenderer->getRenderTarget();

	// Bind G-Buffer as src
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.getID());

	// Bind default buffer as dest
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rTarget);

	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	// Copy src to dest
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);


	glBindFramebuffer(GL_FRAMEBUFFER, rTarget);

	params.entityGroup = &forwardRendererEntityGroup;

	m_forwardRenderer->renderScene(params);

#if 0
	// POST Render Phase
	// Iterate GPU instancing batches
	for (auto&& [entity, mesh, instanceBatch] : m_registry.view<MeshComponent, InstanceBatch>().each())
	{
		// draw model
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = mesh.mesh.get();
		m_deferredRenderer->render(params);
	}

#endif

	// For some reason this group destroys the entities
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	m_skyboxShader->use();
	m_renderTargetFBO->bind();

	for (auto&& [entity, skybox, mesh, transform, mat] : 
		m_registry.view<SkyboxComponent, MeshComponent, Transformation, MaterialComponent>().each())
	{
		Entity entityhandler{ entity, this };
		params.entity = &entityhandler;
		params.mesh = mesh.mesh.get();
		auto tempModel = transform.getWorldTransformation();
		params.model = &tempModel;
		params.shader = (m_skyboxShader.get());
		mat.begin()->get()->getTexture(Texture::Type::Diffuse).get()->bind();

		m_forwardRenderer->render(params); //this causes the many warnings issue

		params.entity = nullptr;
		params.mesh = nullptr;
		params.shader = nullptr;
		params.model = nullptr;
	}
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);


#if 1

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_renderTargetFBO->getID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#endif 

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
	std::string name = "temp";
	return createEntity(name);
}

Entity Scene::createEntity(const std::string& name)
{
	entt::entity e = m_registry.create();
	auto entityHandler = Entity(e, this);
	entityHandler.addComponent<Transformation>(entityHandler);
	entityHandler.addComponent<ObjectComponent>(entityHandler, name); //todo fix
	return entityHandler;
}

void Scene::removeEntity(const Entity& e)
{
	auto id = e.handlerID();
	m_registry.destroy(e.handler());

	logDebug("Removed entity: " + std::to_string(id));
}

ICamera* Scene::getActiveCamera() const
{
	return nullptr;
}


void Scene::clear()
{
	m_registry.clear();
}

Scene::Scene(Context* context)
{
	init(context);
}

std::shared_ptr<IRenderer> Scene::getRenderer() const
{
	return m_deferredRenderer;
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

void Scene::removeActor(entt::entity entity, PhysicsSystem* physicsSystem, RigidBodyComponent& rb)
{
	Entity e{ entity, this };

	auto& rBody = e.getComponent<RigidBodyComponent>();
	m_PhysicsScene->removeActor(*(physx::PxRigidActor*)rBody.simulatedBody);
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
			const std::vector<glm::vec3>& apos = meshComponent->mesh.get()->getPositions();
			shape = physicsSystem->createConvexMeshShape(apos);
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

void Scene::onRigidBodyDestroy(entt::registry& registry, entt::entity entity)
{
	if (m_isSimulationActive)
	{
		Entity e(entity, this);
		auto physicsSystem = Engine::get()->getPhysicsSystem();
		auto& rb = e.getComponent<RigidBodyComponent>();
		removeActor(entity, physicsSystem, rb);
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

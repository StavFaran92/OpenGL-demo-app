#include "Scene.h"

#include "Skybox.h"
#include "Object3D.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Engine.h"
#include "ICamera.h"
#include "Model.h"
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
//#include "Components.h"
#include "Model.h"
#include "InstanceBatch.h"

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

	// Add default dir light
	createEntity()->addComponent<DirectionalLight>();
}

void Scene::update(float deltaTime)
{
	m_renderer->clear();

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

	auto view = m_registry.view<Model>();

	for (auto [entity, model] : view.each())
	{
		model.update(deltaTime);
	}

	//if (m_skybox)
	//	m_skybox->update(deltaTime);
}

void Scene::draw(float deltaTime)
{
	// PRE Render Phase
	{
		Scene::Params params;
		params.scene = this;
		params.registry = &m_registry;
		params.context = m_context;
		params.renderer = m_renderer.get();

		for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_BEGIN])
		{
			cb(&params);
		}

		for (const auto& cb : m_renderCallbacks[RenderPhase::PRE_RENDER_END])
		{
			cb(&params);
		}
	}

	// Render Phase
	for (auto [entity, mesh, transform, renderable] : m_registry.group<Mesh, Transformation, Renderable>().each())
	{
		// If in debug MODE -> put model in displayNormalsQueue
		//if (DEBUG_MODE_ENABLED && DEBUG_DISPLAY_NORMALS)
		//{
		//	m_debugModelDeque.push_back(model);
		//}

		{
			DrawQueueRenderParams params;
			params.scene = this;
			params.mesh = &mesh;
			params.transformation = &transform;
			params.registry = &m_registry;
			for (const auto& cb : m_renderCallbacks[RenderPhase::DRAW_QUEUE_PRE_RENDER])
			{
				cb(&params);
			}
		}

		auto shader = m_registry.try_get<StandardShader>(entity);
		if (shader)
		{
			shader->use();
			shader->updateDirLights(m_registry);
			shader->updatePointLights(m_registry);
			shader->setViewPos(m_renderer->getCamera()->getPosition());
			shader->release();
		}

		// draw model
		Entity entityhandler{ entity, this };
		m_renderer->render(&entityhandler, &mesh, &transform);

		{
			DrawQueueRenderParams params;
			params.scene = this;
			params.mesh = &mesh;
			params.transformation = &transform;
			params.registry = &m_registry;
			for (const auto& cb : m_renderCallbacks[RenderPhase::DRAW_QUEUE_POST_RENDER])
			{
				cb(&params);
			}
		}
	};

	// POST Render Phase
	// Iterate GPU instancing batches
	for (auto [entity, mesh, instanceBatch] : m_registry.group<Mesh, InstanceBatch>().each())
	{
		// draw model
		Entity entityhandler{ entity, this };
		m_gpuInstancingRenderer->render(&entityhandler, &mesh, &instanceBatch);
	}

	//// Draw skybox
	//if (m_skybox)
	//{
	//	m_skybox->draw(*m_skyboxRenderer.get());
	//	m_skybox = nullptr;
	//}

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

	{
		Scene::Params params;
		params.scene = this;
		params.registry = &m_registry;
		params.context = m_context;
		params.renderer = m_renderer.get();

		for (const auto& cb : m_renderCallbacks[RenderPhase::POST_RENDER_BEGIN])
		{
			cb(&params);
		}

		for (const auto& cb : m_renderCallbacks[RenderPhase::POST_RENDER_END])
		{
			cb(&params);
		}
	}
}

//void Scene::drawMultiple(const InstanceBatch& batch)
//{
//	m_instanceBatchQueue.push_back(std::make_shared<InstanceBatch>(batch));
//}

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

std::shared_ptr<Entity> Scene::createEntity()
{
	entt::entity e = m_registry.create();
	auto entityHandler = std::make_shared<Entity>(e, this);
	return entityHandler;
}

void Scene::removeEntity(std::shared_ptr<Entity> e)
{
	m_registry.destroy(e->handler());
}


void Scene::clear()
{
	m_registry.clear();
}

Scene::Scene(Context* context)
{
	init(context);
}

//void Scene::drawSkybox(ObjectHandler<Skybox> skybox)
//{
//	m_skybox = skybox.object();
//}

std::shared_ptr<Renderer> Scene::getRenderer() const
{
	return m_renderer;
}

std::shared_ptr<Renderer> Scene::getSkyboxRenderer()
{
	return m_skyboxRenderer;
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

//Skybox* Scene::getSkybox()
//{
//	return m_skybox;
//}

bool Scene::isPickingPhaseActive() const
{
	return m_objectPicker->isPickingPhaseActive();
}
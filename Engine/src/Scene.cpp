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

void Scene::init(Context* context)
{
	m_context = context;

	auto width = Engine::get()->getWindow()->getWidth();
	auto height = Engine::get()->getWindow()->getHeight();

	m_renderer = std::make_shared<Renderer>();
	m_skyboxRenderer = std::make_shared<SkyboxRenderer>(*m_renderer.get());
	m_gpuInstancingRenderer = std::make_shared<GpuInstancingRenderer>();

	m_objectSelection = std::make_shared<ObjectSelection>();

	m_objectPicker = std::make_shared<ObjectPicker>();
	if (!m_objectPicker->init(width, height))
	{
		logError("Object picker failed to init!");
	}

	m_postProcessProjector = std::make_shared<PostProcessProjector>();
	if (!m_postProcessProjector->init(width, height))
	{
		logError("Post process projector failed to init!");
	}

	m_coroutineManager = std::make_shared<CoroutineSystem>();

	auto light = new DirectionalLight();
	addDirectionalLight(light);

	Engine::get()->getInput()->getMouse()->onMousePressed(Mouse::MouseButton::LeftMousebutton, [&](SDL_Event e)
	{
		m_pickingPhaseActive = true;
	});
}

void Scene::update(float deltaTime)
{
	m_renderer->Clear();

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

	//Update models
	for (auto model = m_models.begin(); model != m_models.end(); ++model)
	{
		model->second->update(deltaTime);
	}

	while (!m_updateQueue.empty())
	{
		auto object = m_updateQueue.front();
		m_updateQueue.pop_front();

		object->update(deltaTime);
	}

	if (m_skybox)
		m_skybox->update(deltaTime);
}

void Scene::draw(float deltaTime)
{


	//// Draw Engine models
	//for (auto model = m_models.begin(); model != m_models.end(); ++model)
	//{
	//	Shader* shader = model->second->getShader();
	//	shader->use();

	//	if (shader->IsLightsEnabled())
	//	{
	//		// Use all directional lights
	//		{
	//			int i = 0;
	//			for (auto it = m_directionalLights.begin(); it != m_directionalLights.end(); ++it, ++i) {
	//				it->second->useLight(*shader, i);
	//			}
	//			shader->SetInt("dirLightCount", m_directionalLights.size());
	//		}

	//		// Use all point lights
	//		{
	//			int i = 0;
	//			for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++i, ++it) {
	//				it->second->useLight(*shader, i);
	//			}
	//			shader->SetInt("pointLightCount", m_pointLights.size());
	//		}
	//	}

	//	glStencilFunc(GL_ALWAYS, model->second->getID(), 0xff);

	//	// Draw model
	//	model->second->draw(*m_renderer.get(), shader);

	//	shader->release();
	//}

	//// Update phong shader
	auto phongShader = m_context->getPhongShader();
	//phongShader->use();
	//phongShader->updateDirLights(m_directionalLights);
	//phongShader->updatePointLights(m_pointLights);
	//phongShader->setViewPos(m_renderer->getCamera()->getPosition());
	//phongShader->release();

	// Picking Phase
	if (m_isObjectSelectionEnabled && m_pickingPhaseActive)
	{
		// Enable writing to picking frame buffer
		m_objectPicker->enableWriting();

		// Set uniforms in picking shader
		auto pickingShader = m_context->getPickingShader();
		pickingShader->use();
		pickingShader->setViewMatrix(m_renderer->getCamera()->getView());
		pickingShader->setProjectionMatrix(m_renderer->getProjection());

		// iterate models queue
		for (unsigned int i = 0; i < m_drawQueue.size(); i++)
		{
			// Set Model related uniforms in picking shader  
			auto model = m_drawQueue[i];
			pickingShader->use();
			pickingShader->setModelMatrix(model->getTransformation()->getMatrix());
			pickingShader->setObjectIndex(model->getID() + 1);
			pickingShader->release();

			// Draw model
			m_renderer->render(model, pickingShader);
			//model->draw(*m_renderer.get(), pickingShader);
		}

		// Release picking shader and stop writing to frame buffer
		pickingShader->release();
		m_objectPicker->disableWriting();

		// Get mouse X & Y
		int x, y;
		Engine::get()->getInput()->getMouse()->getMousePosition(x, y);

		// Pick object in scene according to X & Y
		auto objectID = m_objectPicker->pickObject(x, y);

		// Clears previous object selection
		m_objectSelection->clear();

		// If object returned != -1 then an object has been picked (-1 means background)
		if (objectID != -1)
		{
			auto obj = Engine::get()->getObjectManager()->getObjectById(objectID);
			if (obj)
			{
				obj->pick();
				obj->select();

			}
		}

		// Turn picking phase flag off
		m_pickingPhaseActive = false;
	}

	// Render Phase
	// Post process Enable writing
	if (m_isPostProcessEnabled && m_postProcessProjector)
	{
		m_postProcessProjector->enableWriting();
	}

	// Iterate Application models
	while (!m_drawQueue.empty())
	{
		auto model = m_drawQueue.front();
		m_drawQueue.pop_front();

		// If in debug MODE -> put model in displayNormalsQueue
		if (DEBUG_MODE_ENABLED && DEBUG_DISPLAY_NORMALS)
		{
			m_debugModelDeque.push_back(model);
		}

		auto shader = model->getShader();
		if (shader)
		{
			shader->use();
			shader->updateDirLights(m_directionalLights);
			shader->updatePointLights(m_pointLights);
			shader->setViewPos(m_renderer->getCamera()->getPosition());
			shader->release();
		}

		phongShader->use();

		// If model is selected highlight it's color
		if (m_isObjectSelectionEnabled && isSelected(model->getID()))
			phongShader->setColorMul({ 0.3f, 0.3f, 0.3f, 0.3f });
		else
			phongShader->setColorMul({ 0.f, 0.f, 0.f, 0.f });

		phongShader->release();

		// draw model
		m_renderer->render(model);
		//model->draw(*m_renderer.get());
	}

	// Iterate GPU instancing batches
	while (!m_instanceBatchQueue.empty())
	{
		auto batch = m_instanceBatchQueue.front();
		m_instanceBatchQueue.pop_front();

		// draw model
		m_gpuInstancingRenderer->render(batch);
	}

	// Draw skybox
	if (m_skybox)
	{
		m_skybox->draw(*m_skyboxRenderer.get());
		m_skybox = nullptr;
	}

	if (DEBUG_MODE_ENABLED && DEBUG_DISPLAY_NORMALS)
	{
		// Use normal display shader
		auto normalDisplayShader = m_context->getNormalDisplayShader();

		// draw scene
		while (!m_debugModelDeque.empty())
		{
			auto model = m_debugModelDeque.front();
			m_debugModelDeque.pop_front();

			m_renderer->render(model, normalDisplayShader);
		}
	}

	// Post process disable writing and draw if required
	if (m_isPostProcessEnabled && m_postProcessProjector)
	{
		m_postProcessProjector->disableWriting();
		m_postProcessProjector->draw();
	}
}

void Scene::drawMultiple(const InstanceBatch& batch)
{
	m_instanceBatchQueue.push_back(std::make_shared<InstanceBatch>(batch));
}

void Scene::clear()
{
	m_models.clear();
	m_pointLights.clear();
	m_directionalLights.clear();
}

Scene::Scene(Context* context)
{
	init(context);
}

bool Scene::addModel(Model* model)
{
	m_modelCounter++;
	model->setSceneID(m_modelCounter);
	m_models.emplace(m_modelCounter, std::shared_ptr<Model>(model));

	logInfo("Model {} Added successfully.", std::to_string(m_modelCounter));
	return true;
}

bool Scene::addPointLight(PointLight* pLight)
{
	m_pointLightCounter++;
	pLight->setSceneID(m_pointLightCounter);
	m_pointLights.emplace(m_pointLightCounter, std::shared_ptr<PointLight>(pLight));

	logInfo("PointLight {} Added successfully.", std::to_string(m_pointLightCounter));
	return true;
}

bool Scene::addDirectionalLight(DirectionalLight* dLight)
{
	m_directionalLightCounter++;
	dLight->setSceneID(m_directionalLightCounter);
	m_directionalLights.emplace(m_directionalLightCounter, std::shared_ptr<DirectionalLight>(dLight));

	logInfo("DirectionalLight {} Added successfully.", std::to_string(m_directionalLightCounter));
	return true;
}

bool Scene::removeModel(uint32_t id)
{
	auto iter = m_models.find(id);
	if (iter == m_models.end())
	{
		logError("Could not locate Model {}", id);
		return false;
	}
	m_models.erase(iter);

	logInfo("Model {} Erased successfully.", std::to_string(id));

	return true;
}

bool Scene::removeModel(Model* model)
{
	uint32_t id = model->getID();

	return removeModel(id);
}

bool Scene::removePointLight(PointLight* pLight)
{
	uint32_t id = pLight->getID();
	auto iter = m_pointLights.find(id);
	if (iter == m_pointLights.end())
	{
		logError("Could not locate PointLight {}", id);
		return false;
	}
	m_pointLights.erase(iter);

	logInfo("PointLight {} Erased successfully.", std::to_string(id));

	return true;
}

bool Scene::removeDirectionalLight(DirectionalLight* dLight)
{
	uint32_t id = dLight->getID();
	auto iter = m_directionalLights.find(id);
	if (iter == m_directionalLights.end())
	{
		logError("Could not locate DirectionalLight {}", id);
		return false;
	}
	m_directionalLights.erase(iter);

	logInfo("DirectionalLight {} Erased successfully.", std::to_string(id));

	return true;
}


void Scene::drawSkybox(ObjectHandler<Skybox> skybox)
{
	m_skybox = skybox.object();
}

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
	m_isPostProcessEnabled = value;
}

bool Scene::isSelected(uint32_t id) const
{
	if (!m_isObjectSelectionEnabled)
	{
		logWarning("Object selection isn't enabled for this scene.");
		return false;
	}

	return m_objectSelection->isObjectSelected(id);
}

void Scene::enableObjectSelection(bool isEnabled)
{
	m_isObjectSelectionEnabled = isEnabled;
}

void Scene::selectObject(uint32_t id)
{
	m_objectSelection->selectObject(id);
}

void Scene::clearObjectSelection()
{
	m_objectSelection->clear();
}

void Scene::update(ObjectHandler<Object3D> handler)
{
	m_updateQueue.push_back(handler.object());
}

void Scene::draw(ObjectHandler<Model> handler)
{
	m_drawQueue.push_back(handler.object());
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

Skybox* Scene::getSkybox()
{
	return m_skybox;
}
#include "Context.h"

#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Skybox.h"
#include "SkyboxRenderer.h"
#include "ICamera.h"

#include "Object3D.h"
#include "Scene.h"

#include "Logger.h"

Context::Context() : m_shaderCounter(0)
{
	m_reflectionShader = std::make_shared<Shader>("Resources\\Shaders\\ReflectionShader.vert", "Resources\\Shaders\\ReflectionShader.frag");
	m_reflectionShader->SetEnableLights(false);
	m_reflectionShader->SetEnableMaterials(false);
	m_reflectionShader->SetEnableTextures(false);

	m_refractiveShader = std::make_shared<Shader>("Resources\\Shaders\\RefractionShader.vert", "Resources\\Shaders\\RefractionShader.frag");
	m_refractiveShader->SetEnableLights(false);
	m_refractiveShader->SetEnableMaterials(false);
	m_refractiveShader->SetEnableTextures(false);

	//std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();
	//AddDirectionalLight(light);
}

//bool Context::addObject(std::shared_ptr<Object3D> object)
//{
//	m_objCounter += 1;
//	object->SetID(m_objCounter);
//	m_objects.emplace(m_objCounter, object);
//
//	logInfo("Object {} Added successfully.", std::to_string(m_objCounter));
//
//	return true;
//}
//
//bool Context::removeObject(std::shared_ptr<Object3D> object)
//{
//	uint32_t id = object->getID();
//	auto iter = m_objects.find(id);
//	if (iter == m_objects.end())
//	{
//		logError("Could not locate Object {}", id);
//		return false;
//	}
//	m_objects.erase(iter);
//
//	logInfo("Object {} Erased successfully.", std::to_string(id));
//
//	return true;
//}

bool Context::addScene(std::shared_ptr<Scene> scene)
{
	m_scenesCounter += 1;
	scene->SetID(m_scenesCounter);
	m_scenes.emplace(m_scenesCounter, scene);

	logInfo("Scene {} Added successfully.", std::to_string(m_scenesCounter));

	return true;
}

bool Context::removeScene(std::shared_ptr<Scene> scene)
{
	return false;
}

bool Context::AddShader(std::shared_ptr<Shader> shader)
{
	m_shaderCounter += 1;
	shader->SetID(m_shaderCounter);
	m_shaders.emplace(m_shaderCounter, shader);

	logInfo("Shader {} Added successfully.", std::to_string(m_shaderCounter));

	return true;
}

bool Context::RemoveShader(std::shared_ptr<Shader> shader)
{
	uint32_t uid = shader->getID();
	auto iter = m_shaders.find(uid);
	if (iter == m_shaders.end())
	{
		logError("Could not locate shader {}", uid);
		return false;
	}

	m_shaders.erase(iter);

	logInfo("Shader {} Erased successfully.", std::to_string(uid));

	return true;
}

std::shared_ptr<Shader> Context::GetReflectionShader()
{
	return m_reflectionShader;
}

std::shared_ptr<Shader> Context::GetRefractiveShader()
{
	return m_refractiveShader;
}

void Context::setActiveScene(uint32_t index)
{
	if (index > m_scenesCounter)
	{
		logError("Illegal index specified: " + index);
		return;
	}

	m_activeScene = index;
}

void Context::update(float deltaTime)
{
	if (m_activeScene == -1)
		return;

	m_scenes[m_activeScene]->update(deltaTime);
}

void Context::draw()
{
	if (m_activeScene == -1)
		return;

	m_scenes[m_activeScene]->draw();
}
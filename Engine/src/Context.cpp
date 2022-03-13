#include "Context.h"

#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Skybox.h"
#include "SkyboxRenderer.h"
#include "ICamera.h"

#include "Object3D.h"

Context::Context() : m_objCounter(0), m_shaderCounter(0)
{
	m_renderer = std::make_shared<Renderer>();
	m_skyboxRenderer = std::make_shared<SkyboxRenderer>(*m_renderer.get());

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

bool Context::addObject(std::shared_ptr<Object3D> object)
{
	m_objCounter += 1;
	object->SetID(m_objCounter);
	m_objects.emplace(m_objCounter, object);

	logInfo("Object {} Added successfully.", std::to_string(m_objCounter));

	return true;
}

bool Context::removeObject(std::shared_ptr<Object3D> object)
{
	uint32_t id = object->getID();
	auto iter = m_objects.find(id);
	if (iter == m_objects.end())
	{
		logError("Could not locate Object {}", id);
		return false;
	}
	m_objects.erase(iter);

	logInfo("Object {} Erased successfully.", std::to_string(id));

	return true;
}

//bool Context::AddModel(std::shared_ptr<Model> model)
//{
//	m_modelCounter += 1;
//	model->SetID(m_modelCounter);
//	m_models.emplace(m_modelCounter, model);
//
//	logInfo("Model {} Added successfully.", std::to_string(m_modelCounter));
//
//	return true;
//}
//
//bool Context::RemoveModel(std::shared_ptr<Model> model)
//{
//	uint32_t id = model->getID();
//	auto iter = m_models.find(id);
//	if (iter == m_models.end())
//	{
//		logError("Could not locate model {}", id);
//		return false;
//	}
//
//	m_models.erase(iter);
//
//	logInfo("Model {} Erased successfully.", std::to_string(id));
//
//	return true;
//}
//
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
//
//bool Context::AddPointLight(std::shared_ptr<PointLight> light)
//{
//	m_pointLightCounter += 1;
//	light->SetID(m_pointLightCounter);
//	m_pointLights.emplace(m_pointLightCounter, light);
//
//	logInfo("Point Light {} Added successfully.", std::to_string(m_pointLightCounter));
//
//	return true;
//}
//
//bool Context::RemovePointLight(std::shared_ptr<PointLight> light)
//{
//	uint32_t uid = light->getID();
//	auto iter = m_pointLights.find(uid);
//	if (iter == m_pointLights.end())
//	{
//		logError("Could not locate point light {}", uid);
//		return false;
//	}
//
//	m_pointLights.erase(iter);
//
//	logInfo("Point Light {} Erased successfully.", std::to_string(uid));
//
//	return true;
//}
//
//bool Context::AddDirectionalLight(std::shared_ptr<DirectionalLight> light)
//{
//	m_directionalLightCounter += 1;
//	light->SetID(m_directionalLightCounter);
//	m_directionalLights.emplace(m_directionalLightCounter, light);
//
//	logInfo("Directional Light {} Added successfully.", std::to_string(m_directionalLightCounter));
//
//	return true;
//}
//
//bool Context::RemoveDirectionalLight(std::shared_ptr<DirectionalLight> light)
//{
//	uint32_t uid = light->getID();
//	auto iter = m_directionalLights.find(uid);
//	if (iter == m_directionalLights.end())
//	{
//		logError("Could not locate directional light {}", uid);
//		return false;
//	}
//
//	m_directionalLights.erase(iter);
//
//	logInfo("directional Light {} Erased successfully.", std::to_string(uid));
//
//	return true;
//}
//
//bool Context::AddSkybox(std::shared_ptr<Skybox> skybox)
//{
//	m_skyboxCounter += 1;
//	skybox->SetID(m_skyboxCounter);
//	m_directionalLights.emplace(m_skyboxCounter, skybox);
//
//	logInfo("Skybox {} Added successfully.", std::to_string(m_skyboxCounter));
//
//	return true;
//}

std::shared_ptr<Shader> Context::GetReflectionShader()
{
	return m_reflectionShader;
}

std::shared_ptr<Shader> Context::GetRefractiveShader()
{
	return m_refractiveShader;
}

//std::shared_ptr<Model> Context::GetSkyBox()
//{
//	return m_skybox;
//}



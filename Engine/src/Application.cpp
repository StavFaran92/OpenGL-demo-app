#include "Application.h"

#include "Model.h"
#include "Logger.h"
#include "Engine.h"
#include "Context.h"
#include "Scene.h"
#include "Skybox.h"

void Application::skybox(Skybox*)
{
	auto context = Engine::get()->getContext();
	context->getActiveScene()->setSkybox(Skybox::CreateSkybox());
}

void Application::postProcess(Shader* shader)
{
	auto context = Engine::get()->getContext();
	context->getActiveScene()->setPostProcessShader(shader);
}

void Application::draw(Model* model)
{
	Engine::get()->getContext()->getActiveScene()->draw(model);

}

Model* Application::createBox()
{
	return Model::CreatePrimitiveModel(Model::PrimitiveType::Cube);
}

Model* Application::createQuad()
{
	return Model::CreatePrimitiveModel(Model::PrimitiveType::Quad);
}

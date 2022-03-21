#include "Application.h"

#include "Model.h"
#include "Logger.h"
#include "Engine.h"
#include "Context.h"
#include "Scene.h"



void Application::draw2(Model* model)
{
	Engine::get()->getContext()->getActiveScene()->draw(model);

}

Model* Application::createBox()
{
	return Model::CreatePrimitiveModel(Model::PrimitiveType::Cube);
}

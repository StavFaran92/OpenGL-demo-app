#include "Application.h"

#include "Model.h"
#include "Logger.h"
#include "Engine.h"



void Application::draw2(Model* model)
{


}

Model* Application::createBox()
{
	return Model::CreatePrimitiveModel(Model::PrimitiveType::Cube);
}

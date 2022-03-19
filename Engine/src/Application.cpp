#include "Application.h"

#include "Model.h"
#include "Logger.h"

void Application::draw(Model* model)
{

	logInfo("Draw Model: " + model->getID());
}

#include "Application.h"

#include "Model.h"
#include "Logger.h"
#include "Engine.h"
#include "Context.h"
#include "Scene.h"
#include "Skybox.h"
#include "Input.h"
#include "Mouse.h"
#include "Keyboard.h"

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
	return Model::createPrimitiveModel(Model::PrimitiveType::Cube);
}

Model* Application::createQuad()
{
	return Model::createPrimitiveModel(Model::PrimitiveType::Quad);
}

bool Application::mouse_leftButtonPressed()
{
	return Engine::get()->getInput()->getMouse()->getButtonPressed(Mouse::MouseButtons::LeftMousebutton);
}
bool Application::mouse_rightButtonPressed()
{
	return Engine::get()->getInput()->getMouse()->getButtonPressed(Mouse::MouseButtons::RightMousebutton);
}
bool Application::mouse_middleButtonPressed()
{
	return Engine::get()->getInput()->getMouse()->getButtonPressed(Mouse::MouseButtons::MiddleMousebutton);
}

int Application::keyboard_getKeyState(SDL_Scancode code)
{
	return Engine::get()->getInput()->getKeyboard()->getKeyState(code);
}


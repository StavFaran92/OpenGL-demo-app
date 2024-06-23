#pragma once
#include "sge.h"

class PlayerController : public ScriptableEntity
{
public:
	PlayerController(Entity camera, float moveSpeed) : m_camera(camera), m_movementSpeed(moveSpeed) {};

	void onCreate() override
	{
	}

	void onUpdate(float deltaTime) override
	{
		float velocity = m_movementSpeed * deltaTime;

		auto& camComponent = m_camera.getComponent<CameraComponent>();

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_W))
		{
			entity.getComponent<Transformation>().translate(camComponent.front *velocity);
		}

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_S))
		{
			entity.getComponent<Transformation>().translate(-camComponent.front * velocity);
		}

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_A))
		{
			entity.getComponent<Transformation>().translate(-camComponent.right * velocity);
		}

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_D))
		{
			entity.getComponent<Transformation>().translate(camComponent.right * velocity);
		}
	}

private:
	float m_movementSpeed;
	Entity m_camera;
};

#pragma once

#include "sge.h"

class PlayerController : public ScriptableEntity
{
public:
	PlayerController(Entity camera, float moveSpeed) : m_camera(camera), m_movementSpeed(moveSpeed) {};

	void onCreate() override
	{
	}

	void handleGroundCheck()
	{
		// Check if the player is grounded
		auto& transform = entity.getComponent<Transformation>();
		m_isGrounded = Physics::raycast(transform.getWorldPosition() + glm::vec3(0, -4, 0), glm::vec3(0, -1, 0), 1);
	}

	void applyGravity(float deltaTime)
	{
		if (m_isGrounded)
		{
			m_velocity.y = 0.f; // Reset vertical velocity when grounded
		}
		else
		{
			m_velocity.y +=  -9.81f; // Apply gravity
		}
	}

	void applyMovement(float deltaTime)
	{
		// Apply the computed velocity to move the character
		auto& transform = entity.getComponent<Transformation>();
		auto& rb = entity.getComponent<RigidBodyComponent>();
		glm::vec3 newPosition = transform.getWorldPosition() + (/*movement +*/ m_velocity) / 1000.f;
		rb.move(newPosition);
	}

	void onUpdate(float deltaTime) override
	{
		float velocity = m_movementSpeed * deltaTime;

		auto& camComponent = m_camera.getComponent<CameraComponent>();

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_W))
		{
			//entity.getComponent<Transformation>().translate(camComponent.front *velocity);
			movement = camComponent.front * velocity;
		}

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_S))
		{
			//entity.getComponent<Transformation>().translate(-camComponent.front * velocity);
			movement = -camComponent.front * velocity;
		}

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_A))
		{
			//entity.getComponent<Transformation>().translate(-camComponent.right * velocity);
			movement = -camComponent.right * velocity;
		}

		if (Engine::get()->getInput()->getKeyboard()->getKeyState(SDL_SCANCODE_D))
		{
			//entity.getComponent<Transformation>().translate(camComponent.right * velocity);
			movement = camComponent.right * velocity;
		}

		handleGroundCheck();
		applyGravity(deltaTime);
		applyMovement(deltaTime);
	}

private:
	float m_movementSpeed;
	Entity m_camera;

	bool m_isGrounded = false;

	glm::vec3 m_velocity{};
	glm::vec3 movement{};
};

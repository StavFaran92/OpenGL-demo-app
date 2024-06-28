#pragma once

#include "sge.h"

class EnemyController : public ScriptableEntity
{
public:
	EnemyController(float moveSpeed) : m_movementSpeed(moveSpeed) {};

	void onCreate() override
	{
		m_player = Engine::get()->getContext()->getActiveScene()->getEntityByName("Player");
	}

	void handleGroundCheck()
	{
		// Check if the player is grounded
		auto& transform = entity.getComponent<Transformation>();
		m_isGrounded = Physics::raycast(transform.getWorldPosition() + glm::vec3(0, -1.1, 0), glm::vec3(0, -1, 0), 1.f);
	}

	void applyGravity(float deltaTime)
	{
		if (m_isGrounded && m_velocity.y < 0)
		{
			m_velocity.y = 0.f; // Reset vertical velocity when grounded
		}
		
		if (!m_isGrounded)
		{
			m_velocity.y +=  -9.81f; // Apply gravity
		}
	}

	void applyMovement(float deltaTime)
	{
		// Apply the computed velocity to move the character
		auto& rb = entity.getComponent<RigidBodyComponent>();
		glm::vec3 newPosition = m_movement + glm::vec3(0, m_velocity.y / 1000.f, 0);
		rb.move(newPosition);
	}

	void onUpdate(float deltaTime) override
	{
		float velocity = m_movementSpeed * deltaTime;

		// follow the player
		auto& transform = entity.getComponent<Transformation>();
		auto& playerTransform = m_player.getComponent<Transformation>();
		auto& dir = glm::normalize(playerTransform.getWorldPosition() - transform.getWorldPosition());

		m_movement = glm::vec3(dir.x, 0.f, dir.z) * .01f;


		handleGroundCheck();
		applyGravity(deltaTime);
		applyMovement(deltaTime);
		
	}

private:
	float m_movementSpeed;

	Entity m_player;

	bool m_isGrounded = false;
	bool m_isJumping = false;
	float m_jumpForce = 200.f;

	glm::vec3 m_velocity{};
	glm::vec3 m_movement{};
};

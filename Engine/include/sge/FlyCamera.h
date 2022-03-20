#pragma once
#include "ICamera.h"

#include "Keyboard.h"

class FlyCamera : public ICamera
{
public:
	FlyCamera(glm::vec3 startPosition, float startYaw, float startPitch, float startMoveSpeed, float startTurnSpeed);

	void keyControl(double deltaTime) override;
	void OnMouseMotion(float xChange, float yChange) override;
	void OnMousePressed(SDL_MouseButtonEvent& e) override;
	void OnMouseReleased(SDL_MouseButtonEvent& e) override;

	inline glm::mat4 getView() override { return glm::lookAt(m_position, m_position + m_front, m_up); }
	inline glm::vec3 getPosition() override { return m_position; }

	void update(float deltaTime) override;

	~FlyCamera();
private:
	void calculateOrientation();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;

	float m_movementSpeed;
	float m_turnSpeed;

	std::shared_ptr<Keyboard> keyboard;
};

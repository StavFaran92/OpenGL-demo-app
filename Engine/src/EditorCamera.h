#pragma once

#include "ICamera.h"
#include "Core.h"
#include "Component.h"
#include "ScriptableEntity.h"
#include "ICameraController.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>


class EngineAPI EditorCamera : public ICamera
{
public:
	enum class CameraState
	{
		FreeLook,
		Orbit
	};
public:
	EditorCamera();

	void lookAt(float x, float y, float z) override;
	void setPosition(float distance, float angleX, float angleY) override;
	glm::mat4 getView() override;
	glm::vec3 getPosition() override;

	void onCreate() override;
private:
	bool m_isLocked = true;

	CameraState m_camState = CameraState::FreeLook;

	std::shared_ptr<ICameraController> m_cameraController;

	CameraComponent* m_cameraComponent = nullptr;

};

CEREAL_REGISTER_TYPE(EditorCamera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptableEntity, EditorCamera)
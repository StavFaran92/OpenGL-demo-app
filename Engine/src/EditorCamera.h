#pragma once

#include "ICamera.h"
#include "Core.h"
#include "Component.h"
#include "ScriptableEntity.h"
#include "ICameraController.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>


class EngineAPI EditorCamera : public ScriptableEntity
{
public:
	enum class CameraState
	{
		FreeLook,
		Orbit
	};
public:
	EditorCamera();

	void onCreate() override;
	void onUpdate(float deltaTime) override;
private:
	bool m_isLocked = true;

	CameraState m_camState = CameraState::FreeLook;

	std::shared_ptr<ICameraController> m_cameraController;

	CameraComponent* m_cameraComponent = nullptr;

};

CEREAL_REGISTER_TYPE(EditorCamera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptableEntity, EditorCamera)
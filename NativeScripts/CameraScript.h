#pragma once

#include "sge.h"

#include "FlyCamera.h"

class CameraScript : public ScriptableEntity
{
	virtual void onCreate() override
	{
		Engine::get()->getWindow()->lockMouse();

		m_flyCamera = std::make_shared<FlyCamera>(entity, 0, 0, 3, 1);
	}

	virtual void onDestroy() override
	{
		Engine::get()->getWindow()->unlockMouse();
	}

	virtual void onUpdate(float deltaTime) {
		m_flyCamera->onUpdate(deltaTime);
	};

	std::shared_ptr<FlyCamera> m_flyCamera;
};
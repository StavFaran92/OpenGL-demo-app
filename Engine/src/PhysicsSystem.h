#pragma once

#include "Core.h"

class EngineAPI PhysicsSystem
{
public:
	bool init();

	void createScene();
	void setActiveScene(int index);

	void update(float deltaTime);

private:
	class PhysicsSystemImpl;
	PhysicsSystemImpl* m_pimpl;
};
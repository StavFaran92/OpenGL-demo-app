#pragma once

#include "Core.h"

class EngineAPI PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	bool init();

	void createScene();
	void setActiveScene(int index);

	void update(float deltaTime);

	void close();

private:
	class PhysicsSystemImpl;
	PhysicsSystemImpl* m_pimpl;
};
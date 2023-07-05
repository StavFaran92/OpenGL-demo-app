#pragma once

#include <vector>
#include <cstdint>
#include "Core.h"
#include <PxPhysicsAPI.h>

class EngineAPI PhysicsSystem
{
public:
	PhysicsSystem() = default;
	~PhysicsSystem() = default;

	bool init();

	physx::PxScene* createScene();

	physx::PxPhysics* getPhysics() const;

	physx::PxMaterial* getDefaultMaterial() const;

	void startSimulation();
	void stopSimulation();

	void update(float deltaTime, uint32_t sceneID);

	void close();

private:
	physx::PxDefaultAllocator       m_defaultAllocatorCallback;
	physx::PxDefaultErrorCallback   m_defaultErrorCallback;
	physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
	physx::PxTolerancesScale        m_toleranceScale;

	physx::PxFoundation* m_foundation = nullptr;
	physx::PxPhysics* m_physics = nullptr;

	std::vector<physx::PxScene*>    m_scenes;

	physx::PxMaterial* m_defaultMaterial = nullptr;

	physx::PxPvd* mPvd = nullptr;

	bool m_isSimulationActive = false;
};
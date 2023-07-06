#pragma once

#include <vector>
#include <cstdint>
#include "Core.h"
#include <PxPhysicsAPI.h>

#include "Transformation.h"
#include "Configurations.h"

class EngineAPI PhysicsSystem
{
public:
	PhysicsSystem() = default;
	~PhysicsSystem() = default;

	bool init();

	physx::PxScene* createScene();

	physx::PxPhysics* getPhysics() const;

	physx::PxMaterial* getDefaultMaterial() const;

	physx::PxRigidActor* createRigidBody(Transformation& trasform, RigidbodyType bodyType, float mass);

	physx::PxShape* createBoxShape(float x, float y, float z);

	physx::PxShape* createSphereShape(float radius);

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

	physx::PxPvd* m_pvd = nullptr;

	bool m_isSimulationActive = false;

	bool m_isInit = false;
};
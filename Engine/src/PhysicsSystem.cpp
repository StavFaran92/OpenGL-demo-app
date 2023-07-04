#include "PhysicsSystem.h"

#include "Logger.h"

bool PhysicsSystem::init()
{
    // init physx
    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocatorCallback, m_defaultErrorCallback);
    if (!m_foundation)
    {
        logError("PxCreateFoundation failed!");
        return false;
    }

#ifdef SGE_DEBUG
    mPvd = PxCreatePvd(*m_foundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif // SGE_DEBUG

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, mPvd);

    // Set up cooking parameters
    physx::PxCookingParams cookingParams(m_physics->getTolerancesScale());

    cookingParams.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
    cookingParams.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
    cookingParams.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eWELD_VERTICES;

    physx::PxCooking* cooking = PxCreateCooking(PX_PHYSICS_VERSION, m_physics->getFoundation(), cookingParams);
    if (!cooking)
    {
        logError("PxCreateCooking failed!");
        return false;
    }

    m_defaultMaterial = m_physics->createMaterial(0.5f, 0.5f, 0.f);
}

void PhysicsSystem::update(float deltaTime, uint32_t sceneID)
{
    m_scenes[sceneID]->simulate(deltaTime);
    m_scenes[sceneID]->fetchResults(true);
}

physx::PxScene* PhysicsSystem::createScene()
{
    physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_dispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    physx::PxScene* scene = m_physics->createScene(sceneDesc);
    return scene;
}

void PhysicsSystem::startSimulation()
{
    if (m_isSimulationActive)
    {
        logWarning("Simulation already active.");
        return;
    }

    m_isSimulationActive = true;
}

void PhysicsSystem::stopSimulation()
{
    if (!m_isSimulationActive)
    {
        logWarning("Simulation already stopped.");
        return;
    }

    m_isSimulationActive = false;
}

void PhysicsSystem::close()
{
    m_physics->release();
    m_foundation->release();
}
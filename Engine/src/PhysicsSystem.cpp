#include "PhysicsSystem.h"

#include "Logger.h"
#include <PxPhysicsAPI.h>

class PhysicsSystem::PhysicsSystemImpl
{
public:
    bool init();

    void createScene();
    void setActiveScene(int index);

    void update(float deltaTime);
private:
    physx::PxDefaultAllocator       m_defaultAllocatorCallback;
    physx::PxDefaultErrorCallback   m_defaultErrorCallback;
    physx::PxDefaultCpuDispatcher*  m_dispatcher = nullptr;
    physx::PxTolerancesScale        m_toleranceScale;

    physx::PxFoundation*            m_foundation = nullptr;
    physx::PxPhysics*               m_physics = nullptr;

    std::vector<physx::PxScene*>    m_scenes;
    int m_activeScene = -1;
    physx::PxMaterial*              mMaterial = nullptr;

    physx::PxPvd*                   mPvd = nullptr;
};

bool PhysicsSystem::PhysicsSystemImpl::init()
{
    // init physx
    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocatorCallback, m_defaultErrorCallback);
    if (!m_foundation)
    {
        logError("PxCreateFoundation failed!");
        return false;
    }

#ifdef DEBUG
    mPvd = PxCreatePvd(*m_foundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif // DEBUG

    m_toleranceScale.length = 100;        // typical length of an object
    m_toleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, m_toleranceScale, true, mPvd);

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

#ifdef DEBUG
    physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
#endif // DEBUG
}

void PhysicsSystem::PhysicsSystemImpl::update(float deltaTime)
{
    if (m_activeScene != -1)
    {
        m_scenes[m_activeScene]->simulate(deltaTime);
        m_scenes[m_activeScene]->fetchResults(true);
    }
}

void PhysicsSystem::PhysicsSystemImpl::createScene()
{
    physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_dispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    physx::PxScene* scene = m_physics->createScene(sceneDesc);
    m_scenes.push_back(scene);
    setActiveScene(m_scenes.size() - 1);
}

void PhysicsSystem::PhysicsSystemImpl::setActiveScene(int index)
{
    if (index > m_scenes.size() || index < 0)
    {
        logError("Invalid index specified: " + std::to_string(index));
        return;
    }

    m_activeScene = index;
}


bool PhysicsSystem::init()
{
    return m_pimpl->init();
}

void PhysicsSystem::update(float deltaTime)
{
    m_pimpl->update(deltaTime);
}

void PhysicsSystem::createScene()
{
    m_pimpl->createScene();
}

void PhysicsSystem::setActiveScene(int index)
{
    m_pimpl->setActiveScene(index);
}
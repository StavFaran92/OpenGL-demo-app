#include "PhysicsSystem.h"

#include "Logger.h"

#include "PhysXUtils.h"
#include "Component.h"

#define PX_RELEASE(x)	if(x)	{ x->release(); x = nullptr; }

bool PhysicsSystem::init()
{
    if (m_isInit)
        return false;;

    // init physx
    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocatorCallback, m_defaultErrorCallback);
    if (!m_foundation)
    {
        logError("PxCreateFoundation failed!");
        return false;
    }

#ifdef SGE_DEBUG
    m_pvd = PxCreatePvd(*m_foundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif // SGE_DEBUG

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, m_pvd);

    // Set up cooking parameters
    physx::PxCookingParams cookingParams(m_physics->getTolerancesScale());

    cookingParams.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
    cookingParams.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
    cookingParams.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eWELD_VERTICES;

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, m_physics->getFoundation(), cookingParams);
    if (!m_cooking)
    {
        logError("PxCreateCooking failed!");
        return false;
    }

    m_defaultMaterial = m_physics->createMaterial(0.5f, 0.5f, 0.f);

    m_isInit = true;

    return true;
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

physx::PxPhysics* PhysicsSystem::getPhysics() const
{
    return m_physics;
}

physx::PxMaterial* PhysicsSystem::getDefaultMaterial() const
{
    return m_defaultMaterial;
}

physx::PxRigidActor* PhysicsSystem::createRigidBody(Transformation& transform, RigidbodyType bodyType, float mass)
{
    auto scale = transform.getScale();
    physx::PxTransform pxTransform = PhysXUtils::toPhysXTransform(transform);
    physx::PxRigidActor* body = nullptr;

    if (bodyType == RigidbodyType::Dynamic)
    {
        body = m_physics->createRigidDynamic(pxTransform);
        auto dynamicBody = static_cast<physx::PxRigidDynamic*>(body);
        dynamicBody->setAngularDamping(0.5f);
        physx::PxRigidBodyExt::updateMassAndInertia(*dynamicBody, mass);
    }
    else if (bodyType == RigidbodyType::Static)
    {
        body = m_physics->createRigidStatic(pxTransform);
    }

    assert(body);

    return body;

}

physx::PxShape* PhysicsSystem::createBoxShape(float x, float y, float z)
{
    return m_physics->createShape(physx::PxBoxGeometry(x, y, z), *m_defaultMaterial);
}

physx::PxShape* PhysicsSystem::createSphereShape(float radius)
{
    return m_physics->createShape(physx::PxSphereGeometry(radius), *m_defaultMaterial);
}

physx::PxShape* PhysicsSystem::createConvexMeshShape(const std::vector<glm::vec3>& vertices)
{
    physx::PxConvexMeshDesc convexDesc;
    convexDesc.points.count = vertices.size();
    convexDesc.points.stride = sizeof(glm::vec3);
    convexDesc.points.data = vertices.data();
    convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    physx::PxDefaultMemoryOutputStream buf;
    if (!m_cooking->cookConvexMesh(convexDesc, buf))
        return nullptr;

    physx::PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
    physx::PxConvexMesh* convexMesh = m_physics->createConvexMesh(id);

    physx::PxShape* convexShape = m_physics->createShape(physx::PxConvexMeshGeometry(convexMesh), *m_defaultMaterial, true);

    return convexShape;
}

void PhysicsSystem::renderWireframeDebug()
{

}


void PhysicsSystem::close()
{
    if (!m_isInit)
        return;

    PX_RELEASE(m_dispatcher);
    PX_RELEASE(m_physics);
    if (m_pvd)
    {
        physx::PxPvdTransport* transport = m_pvd->getTransport();
        PX_RELEASE(m_pvd);
        PX_RELEASE(transport);
    }
    PX_RELEASE(m_foundation);
}
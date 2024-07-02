#include "Physics.h"

#include "Engine.h"
#include "Context.h"
#include "Scene.h"

#include <PxPhysicsAPI.h>

using namespace physx;

bool Physics::raycast(glm::vec3 origin, glm::vec3 dir, float distance, LayerMask mask)
{
    PxScene* scene = Engine::get()->getContext()->getActiveScene()->getPhysicsScene();
    PxRaycastBuffer hit;

    const PxHitFlags outputFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL;

    // [in] Raycast against GROUP2 and GROUP3
    PxQueryFilterData filterData = PxQueryFilterData();
    filterData.data.word0 = mask;

    if (scene->raycast(PxVec3(origin.x, origin.y, origin.z), PxVec3(dir.x, dir.y, dir.z), distance, hit, outputFlags, filterData))
    {
        return true;
    }

    return false;
}

#include "Physics.h"

#include "Engine.h"
#include "Context.h"
#include "Scene.h"

#include <PxPhysicsAPI.h>

using namespace physx;

bool Physics::raycast(glm::vec3 origin, glm::vec3 dir, float distance)
{
    PxScene* scene = Engine::get()->getContext()->getActiveScene()->getPhysicsScene();
    PxRaycastBuffer hit;

    if (scene->raycast(PxVec3(origin.x, origin.y, origin.z), PxVec3(dir.x, dir.y, dir.z), distance, hit))
    {
        return true;
    }

    return false;
}

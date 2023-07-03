#pragma once

#include "Transformation.h"
#include <PxPhysicsAPI.h>

class PhysXUtils
{
public:
	inline static physx::PxTransform toPhysXTransform(const Transformation& transform)
	{
		auto translation = transform.getPosition();
		auto orientation = transform.getWorldOrientation();

		physx::PxVec3 pxTranslation(translation.x, translation.y, translation.z);
		physx::PxQuat pxRotation(orientation.x, orientation.y, orientation.z, orientation.w);

		return physx::PxTransform(pxTranslation, pxRotation);
	}

	inline static void toPhysXTransform(const Transformation& transform, physx::PxTransform& pxTransform)
	{
		auto translation = transform.getPosition();
		auto orientation = transform.getWorldOrientation();

		physx::PxVec3 pxTranslation(translation.x, translation.y, translation.z);
		physx::PxQuat pxRotation(orientation.x, orientation.y, orientation.z, orientation.w);

		pxTransform.p = pxTranslation;
		pxTransform.q = pxRotation;
	}

	inline static Transformation fromPhysXTransform(const Entity& entity, const physx::PxTransform& pxTransform)
	{
		glm::vec3 translation(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z);
		glm::quat rotation(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w);

		return Transformation(entity, translation, rotation);
	}

	inline static void fromPhysXTransform(const Entity& entity, const physx::PxTransform& pxTransform, Transformation& transform)
	{
		glm::vec3 translation(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z);
		glm::quat rotation(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w);

		transform.setPosition(translation);
		transform.setRotation(rotation);
	}
};
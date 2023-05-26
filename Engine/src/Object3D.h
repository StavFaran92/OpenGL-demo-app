#pragma once

#include <memory>
#include <vector>
#include "Core.h"
#include "glm/glm.hpp"

#include "ISelectable.h"
#include "IPickable.h"
#include "entt/entt.hpp"

class ObjectManager;
class ObjectFactory;
class Transformation;
class Scene;

class EngineAPI Object3D : public ISelectable, public IPickable
{
public:
	virtual ~Object3D() = default;

	Transformation* getTransformation() const;
	uint32_t getID() const;

	void translate(float x, float y, float z) const;
	void translate(glm::vec3 translation) const;
	void scale(float x, float y, float z) const;
	void scale(glm::vec3 scaleFactor) const;
	void rotate(glm::vec3 eulers) const;

	/**
	 * @brief Rotates a 3D vector around a given axis.
	 *
	 * This function rotates the current 3D vector around the specified axis.
	 * The rotation is performed in a right-hand rule manner,
	 * i.e., if you curl the fingers of your right hand in the direction of rotation,
	 * your thumb points in the direction of the axis.
	 *
	 * @param axis The axis around which the vector will be rotated. This should be a normalized vector.
	 * @param angle The angle by which the vector will be rotated, in degrees. Positive values denote
	 *              counterclockwise rotation when looking from the origin towards the positive end of the axis.
	 */
	void rotate(glm::vec3 axis, float angle) const;
	void rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle) const;
	void rotateLerp(glm::vec3 axis, float angle, float t) const;
	void rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t) const;

	void addChildren(std::shared_ptr<Object3D> child);
	void addChildren(Object3D* child);

	void removeChildren(std::shared_ptr<Object3D> child);
	void removeChildren(Object3D* child);

	void select() override;
	void pick() override;

	void onSelected() override;
	void onPicked() override;

	//template<typename T, typename... Args>
	//T& addComponent(Args&&... args)
	//{
	//	//HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
	//	
	//	m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
	//	return entity;
	//}

	//template<typename T>
	//T& getComponent()
	//{
	//	//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
	//	return m_registry.get<T>(m_entity);
	//}

	//template<typename T>
	//bool HasComponent()
	//{
	//	return m_Scene->m_Registry.has<T>(m_entity);
	//}

	//template<typename T>
	//void RemoveComponent()
	//{
	//	//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
	//	m_Scene->m_Registry.remove<T>(m_entity);
	//}

protected:
	friend class Scene;
	void setSceneID(uint32_t id);
	virtual void update(float deltatime);

protected:
	friend class ObjectFactory;
	Object3D();

	std::shared_ptr<Transformation> m_transformation;

	std::vector<Object3D*> m_childrens;

private:
	friend class ObjectManager;

	uint32_t m_sceneID = 0;
	uint32_t m_id = 0;

	
	entt::entity m_entity;
};

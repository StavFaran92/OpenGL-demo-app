#pragma once

#include "Core.h"
#include "entt/entt.hpp"
#include "Scene.h"

/**
 * @class Entity
 * @brief Class for handling entities in a scene
 *
 * This class provides an interface for creating, adding, getting, and removing components
 * for entities in a scene.
 */
class EngineAPI Entity
{
public:
    /// Default constructor
    Entity() = delete;

    /**
     * @brief Constructs an entity belonging to a scene
     * @param scene Pointer to the scene the entity belongs to
     */
    Entity(entt::entity e, Scene* scene)
        : m_entity(e), m_scene(scene)
    {}

    ~Entity() {};

    /**
     * @brief Adds a component to the entity by moving an existing instance
     * @tparam T Type of the component
     * @param componentInstance Pointer to the component instance
     * @return Reference to the added component
     * @warning The behavior of the existing componentInstance after the call is undefined
     */
    template<typename T>
    T& addComponent(T* componentInstance)
    {
        assert(valid() && "Invalid entity.");
        assert(!m_scene->getRegistry().has<T>(m_entity) && "Component already exists.");
        T& component = m_scene->getRegistry().emplace<T>(m_entity, *componentInstance);
        return component;
    }

    /**
     * @brief Constructs and adds a new component to the entity
     * @tparam T Type of the component
     * @tparam Args Types of arguments to forward to the constructor of the component
     * @param args Arguments to forward to the constructor of the component
     * @return Reference to the added component
     */
    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        assert(valid() && "Invalid entity.");
        assert(!m_scene->getRegistry().has<T>(m_entity) && "Component already exists.");
        T& component = m_scene->getRegistry().emplace<T>(m_entity, std::forward<Args>(args)...);
        return component;
    }

    /**
     * @brief Retrieves a component from the entity
     * @tparam T Type of the component
     * @return Reference to the requested component
     */
    template<typename T>
    T& getComponent()
    {
        assert(valid() && "Invalid entity.");
        assert(m_scene->getRegistry().has<T>(m_entity) && "Component does not exist.");
        return m_scene->getRegistry().get<T>(m_entity);
    }

    /**
     * @brief Checks if the entity has a specific component
     * @tparam T Type of the component
     * @return true if the entity has the component, false otherwise
     */
    template<typename T>
    bool HasComponent()
    {
        assert(valid() && "Invalid entity.");
        return m_scene->getRegistry().has<T>(m_entity);
    }

    /**
     * @brief Removes a component from the entity
     * @tparam T Type of the component
     */
    template<typename T>
    void RemoveComponent()
    {
        assert(valid() && "Invalid entity.");
        assert(m_scene->getRegistry().has<T>(m_entity) && "Component does not exist.");
        m_scene->getRegistry().remove<T>(m_entity);
    }

    /**
     * @brief Checks if the entity is valid
     * @return true if the entity is valid, false otherwise
     */
    bool valid() const
    {
        return m_scene != nullptr && m_entity != entt::null && m_scene->getRegistry().valid(m_entity);
    }

private:
    inline entt::entity handler() const
    {
        return m_entity;
    }



    friend class Scene;
private:
    Scene* m_scene = nullptr;  ///< Scene the entity belongs to
    entt::entity m_entity{ entt::null };  ///< ENTT entity instance
};
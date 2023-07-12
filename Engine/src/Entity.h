#pragma once

#include "Core.h"
#include "entt/entt.hpp"
#include "Scene.h"
//#include "Component.h"


using entity_id = entt::id_type;
struct NativeScriptComponent;

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
    static Entity EmptyEntity;
    /// Default constructor
    Entity() = default;

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

        if constexpr (std::is_same_v<T, NativeScriptComponent>)
        {
            componentInstance->entity = Entity(m_entity, m_scene);
        }

        T& component = m_scene->getRegistry().emplace_or_replace<T>(m_entity, *componentInstance);

        m_components.insert(typeid(T).name());

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

        T& component = m_scene->getRegistry().emplace_or_replace<T>(m_entity, std::forward<Args>(args)...);

        if constexpr (std::is_same_v<T, NativeScriptComponent>)
        {
            component.entity = Entity(m_entity, m_scene);
        }

        m_components.insert(typeid(T).name());

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

    template<typename T>
    T* tryGetComponent()
    {
        assert(valid() && "Invalid entity.");
        return m_scene->getRegistry().try_get<T>(m_entity);
    }

    template<typename T>
    T* tryGetComponentInParent(bool includeSelf = true)
    {
        assert(valid() && "Invalid entity.");
        if (includeSelf)
        {
            auto comp = tryGetComponent<T>();
            if (comp)
            {
                return comp;
            }
        }

        auto parent = getParent();
        if (parent.valid())
        {
            return parent.tryGetComponentInParent<T>(true);
        }

        return nullptr;
    }

    template<typename T>
    T& getComponentInParent(bool includeSelf = true)
    {
        assert(valid() && "Invalid entity.");
        if (includeSelf)
        {
            if (HasComponent<T>())
            {
                return getComponent<T>();
            }
        }

        auto parent = getParent();
        if (parent.valid())
        {
            return parent.getComponentInParent<T>(true);
        }

        assert(false && "Component not found in tree hierarchy.");

        throw std::runtime_error("Component not found in tree hierarchy.");
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
        return m_scene->getRegistry().any_of<T>(m_entity);
    }

    /**
     * @brief Removes a component from the entity
     * @tparam T Type of the component
     */
    template<typename T>
    void RemoveComponent()
    {
        assert(valid() && "Invalid entity.");
        m_scene->getRegistry().remove<T>(m_entity);

        m_components.erase(typeid(T).name());

    }

    

    void setParent(Entity entity);
    Entity removeParent();
    Entity getParent();
    void addChildren(Entity entity);
    void removeChildren(Entity entity);
    std::unordered_map<entity_id, Entity> getChildren();

    /**
     * @brief Checks if the entity is valid
     * @return true if the entity is valid, false otherwise
     */
    bool valid() const
    {
        return m_scene != nullptr && m_entity != entt::null && m_scene->getRegistry().valid(m_entity);
    }

    bool operator==(const Entity& other)
    {
        return m_scene == other.m_scene && m_entity == other.m_entity;
    }

    bool operator!=(const Entity& other)
    {
        return m_scene != other.m_scene || m_entity != other.m_entity;
    }

    inline entt::entity handler() const
    {
        return m_entity;
    }

    inline entity_id handlerID() const
    {
        return (entity_id)m_entity;
    }

private:
    Scene* m_scene = nullptr;  ///< Scene the entity belongs to
    entt::entity m_entity{ entt::null };  ///< ENTT entity instance

    // TODO remove
    std::set<std::string> m_components;

};


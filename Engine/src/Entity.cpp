#include "Entity.h"

#include "Component.h"
#include <iostream>

Entity Entity::EmptyEntity{};

void Entity::setParent(Entity entity)
{
    assert(valid() && "Invalid entity.");
    assert(entity->valid() && "Invalid parent entity.");
    assert(entity->HasComponent<HierarchyComponent>() && "Entity does not contain HierarchyComponent.");
    auto& hierarchy = getComponent<HierarchyComponent>();
    hierarchy.parent = entity;
}

Entity Entity::getParent()
{
    assert(HasComponent<HierarchyComponent>() && "Entity does not contain HierarchyComponent.");
    auto& hierarchy = getComponent<HierarchyComponent>();
    return hierarchy.parent;
}

// Specialized version for ScriptableEntity
template<>
ScriptableEntity& Entity::getComponent<ScriptableEntity>()
{
    assert(valid() && "Invalid entity.");
    assert(m_scene->getRegistry().has<NativeScriptComponent>(m_entity) && "Component does not exist.");
    auto script = m_scene->getRegistry().get<NativeScriptComponent>(m_entity).script;
    assert(script && "No script available.");
    return *script;
}

template<>
NativeScriptComponent& Entity::addComponent(NativeScriptComponent* nsc)
{
    assert(valid() && "Invalid entity.");
    NativeScriptComponent& component = m_scene->getRegistry().emplace_or_replace<NativeScriptComponent>(m_entity, *nsc);
    component.script->entity = Entity(m_entity, m_scene);

    std::cout << "test \n";

    m_components.insert(typeid(nsc).name());

    return component;
}

Entity Entity::removeParent()
{
    auto parent = getParent();
    setParent(EmptyEntity);
    return parent;
}

void Entity::addChildren(Entity entity)
{
    assert(HasComponent<HierarchyComponent>() && "Entity does not contain HierarchyComponent.");
    auto& hierarchy = getComponent<HierarchyComponent>();
    hierarchy.children[entity.handlerID()] = entity;
}
void Entity::removeChildren(Entity entity)
{
    assert(HasComponent<HierarchyComponent>() && "Entity does not contain HierarchyComponent.");
    auto& hierarchy = getComponent<HierarchyComponent>();
    hierarchy.children.erase(entity.handlerID());
}

auto Entity::getChildren()
{
    assert(HasComponent<HierarchyComponent>() && "Entity does not contain HierarchyComponent.");
    auto& hierarchy = getComponent<HierarchyComponent>();
    return hierarchy.children;
}
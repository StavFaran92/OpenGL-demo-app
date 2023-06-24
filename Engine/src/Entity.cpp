#include "Entity.h"

#include "Component.h"

void Entity::setParent(Entity* entity)
{
    assert(valid() && "Invalid entity.");
    assert(entity->valid() && "Invalid parent entity.");
    assert(entity->HasComponent<HierarchyComponent>() && "Entity does not contain HierarchyComponent.");
    auto& hierarchy = getComponent<HierarchyComponent>();
    hierarchy.parent = entity->handler();
}

std::shared_ptr<Entity> Entity::getParent()
{
    assert(HasComponent<HierarchyComponent>() && "Entity does not contain HierarchyComponent.");
    auto& hierarchy = getComponent<HierarchyComponent>();
    if (hierarchy.parent == entt::null)
    {
        return nullptr;
    }

    return std::make_shared<Entity>(hierarchy.parent, m_scene);
}
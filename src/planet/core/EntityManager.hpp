#pragma once

#include <queue>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

class EntityManager
{
public:
    EntityManager(entt::registry& _registry);
    ~EntityManager() = default;

    // Normally it's ok to destroy entities during a view update,
    // but in some cases we need to destroy an entity which is not currenly updated.
    // For this case we need to schedule the entity destruction for later.
    void destroyEntities();
    void registerEntityForDestruction(entt::entity _entity);

private:
    entt::registry& m_registryRef;
    std::queue<entt::entity> m_entitiesToDestroy;
};

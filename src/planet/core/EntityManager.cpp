#include "core/EntityManager.hpp"


EntityManager::EntityManager(entt::registry& _registry)
    : m_registryRef{_registry}
{
}

void EntityManager::destroyEntities()
{
    while (!m_entitiesToDestroy.empty())
    {
        auto entity = m_entitiesToDestroy.front();
        if (m_registryRef.valid(entity))
            m_registryRef.destroy(entity);
        m_entitiesToDestroy.pop();
    }
}

void EntityManager::registerEntityForDestruction(entt::entity _entity)
{
    m_entitiesToDestroy.push(_entity);
}

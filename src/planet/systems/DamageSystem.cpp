#include "systems/DamageSystem.hpp"
#include "events/Events.hpp"
#include "components/Components.hpp"
#include "core/Engine.hpp"
#include "core/Utils.hpp"
#include "EntityType.hpp"

void DamageSystem::onInit()
{
    m_dispatcherRef->sink<CollisionEvent>().connect<&DamageSystem::receiveCollisionEvent>(this);
}

void DamageSystem::receiveCollisionEvent(const CollisionEvent& _event)
{
    if (m_registryRef->all_of<Shootable>(_event.entityA) || m_registryRef->all_of<Shootable>(_event.entityB))
    {
        if (m_registryRef->all_of<Damageable>(_event.entityA) || m_registryRef->all_of<Damageable>(_event.entityB))
        {
            m_systemManagerRef->getEntityManager().registerEntityForDestruction(_event.entityA);
            m_systemManagerRef->getEntityManager().registerEntityForDestruction(_event.entityB);
        }
    }
}

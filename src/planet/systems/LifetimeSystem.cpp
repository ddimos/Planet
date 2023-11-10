#include "systems/LifetimeSystem.hpp"
#include "components/Components.hpp"
#include "core/Engine.hpp"


void LifetimeSystem::onUpdate(float _dt)
{
    for(auto &&[entity, lifetimeComp] : m_registryRef->view<LifetimeComponent>().each())
    {
        lifetimeComp.lifetimeS -= _dt;
        if (lifetimeComp.lifetimeS <= 0.f)
        {
            m_registryRef->destroy(entity);
        }
    }
}

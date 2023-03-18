#include "systems/BaseSystem.hpp"
#include "core/SystemManager.hpp"


void BaseSystem::init(SystemManager& _systemManagerRef)
{
    m_engineRef = &_systemManagerRef.getEngine();
    m_systemManagerRef = &_systemManagerRef;
    m_registryRef = &_systemManagerRef.getRegistry();
    m_dispatcherRef = &_systemManagerRef.getDispatcher();

    onInit();
}

void BaseSystem::update(float _dt)
{
    onUpdate(_dt);
}


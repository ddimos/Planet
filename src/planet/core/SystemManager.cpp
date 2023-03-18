#include "core/SystemManager.hpp"
#include "core/Engine.hpp"
#include "systems/BaseSystem.hpp"


SystemManager::SystemManager(Engine& _engineRef)
    : m_engineRef(_engineRef)
{
}

SystemManager::~SystemManager() = default;

void SystemManager::init()
{
    for (auto& system : m_systems)
        system->init(*this);
    for (auto& system : m_renderSystems)
        system->init(*this);
}

void SystemManager::addSystem(std::unique_ptr<BaseSystem> _system)
{
    m_systems.emplace_back(std::move(_system));
}

void SystemManager::addRenderSystem(std::unique_ptr<BaseSystem> _system)
{
    m_renderSystems.emplace_back(std::move(_system));
}

void SystemManager::update(float _dt)
{
    for (auto& system : m_systems)
        system->update(_dt);
}

void SystemManager::render()
{
    for (auto& system : m_renderSystems)
        system->update(0.f);
}
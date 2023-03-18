#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

class Engine;
class SystemManager;

class BaseSystem
{
public:
    BaseSystem() = default;
    virtual ~BaseSystem() = default;

    void init(SystemManager& _systemManagerRef);
    void update(float _dt);

protected:
    virtual void onInit() {}
    virtual void onUpdate(float _dt) {}

    Engine* m_engineRef = nullptr;
    SystemManager* m_systemManagerRef = nullptr;
    entt::registry* m_registryRef = nullptr;
    entt::dispatcher* m_dispatcherRef = nullptr;
};

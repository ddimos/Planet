#pragma once

#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>


class Engine;
class BaseSystem;

class SystemManager
{
public:
    SystemManager(Engine& _engineRef);
    ~SystemManager();

    void init();
    void update(float _dt);
    void render();

    void addSystem(std::unique_ptr<BaseSystem>);
    void addRenderSystem(std::unique_ptr<BaseSystem>);

    Engine& getEngine() { return m_engineRef; }
    entt::registry& getRegistry() { return m_registry; }
    entt::dispatcher& getDispatcher() { return m_dispatcher; }

private:
    Engine& m_engineRef;
    entt::registry m_registry;
    entt::dispatcher m_dispatcher;

    std::vector<std::unique_ptr<BaseSystem>> m_systems;
    std::vector<std::unique_ptr<BaseSystem>> m_renderSystems;
};
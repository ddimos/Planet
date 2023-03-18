#pragma once
#include "core/SystemManager.hpp"

namespace sf
{
    class RenderWindow;
}
class Engine;

class Game
{
public:
    Game(Engine& _engineRef);
    ~Game() = default;

    void init();
    void update();
    void fixedUpdate(float _dt);
    void render();

    SystemManager& getSystemManager() { return m_systemManager; }

private:
    void handleEvents();

    Engine& m_engineRef;
    sf::RenderWindow& m_window;

    SystemManager m_systemManager;
};


#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Game.hpp"
#include "core/ResourceManager.hpp"

class Engine
{
public:
    Engine();
    ~Engine() = default;

    void start();
    void stop();

    sf::RenderWindow& getWindow() { return m_window; }
    ResourceManager& getResourceManager() { return m_resourceManager; }
    Game& getGame() { return m_game; }

private:
    bool m_isRunning = true;

    sf::RenderWindow m_window;
    ResourceManager m_resourceManager;

    Game m_game;
};
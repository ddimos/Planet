#include "Game.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "core/Engine.hpp"
#include "core/SystemManager.hpp"
#include "core/Log.hpp"

#include "components/Components.hpp"

#include "systems/RenderSystem.hpp"
#include "systems/PlayerSystem.hpp"

Game::Game(Engine& _engineRef)
    : m_engineRef(_engineRef)
    , m_windowRef(_engineRef.getWindow())
    , m_systemManager(_engineRef)
{
}

void Game::init()
{
    auto& resourceManager = m_engineRef.getResourceManager();
    resourceManager.loadTexture("res/textures/earth.png", "earth");
    resourceManager.loadTexture("res/textures/sky_stars.jpg", "sky_stars");
    resourceManager.loadTexture("res/textures/player_front.png", "player_front");

    m_systemManager.addSystem(std::make_unique<PlayerSystem>());

    m_systemManager.addRenderSystem(std::make_unique<RenderSystem>(m_windowRef));

    m_systemManager.init();

    auto& registry = m_systemManager.getRegistry();

    auto planet = registry.create();
    auto& transform = registry.emplace<Transform>(planet);
    transform.position = sf::Vector2f(0.f, 284.f);
    auto& renderable = registry.emplace<Renderable>(planet);
    renderable.sprite.setTexture(resourceManager.getTexture("earth"));
    renderable.sprite.setPosition(transform.position);
    {
        auto player = registry.create();
        registry.emplace<Player>(player);
        registry.emplace<Transform>(player);
        auto& renderable = registry.emplace<Renderable>(player);
        renderable.sprite.setTexture(resourceManager.getTexture("player_front"));
        renderable.sprite.setPosition(sf::Vector2f(0.f, 0.f));
    }
    {
        auto background = registry.create();
        registry.emplace<Transform>(background);
        auto& renderable = registry.emplace<Renderable>(background);
        renderable.sprite.setTexture(resourceManager.getTexture("sky_stars"));
        renderable.sprite.setPosition(sf::Vector2f(0.f, 0.f));
    }
}

void Game::update()
{
    handleEvents();

}
void Game::fixedUpdate(float _dt)
{
    m_systemManager.update(_dt);
}

void Game::render()
{
    m_systemManager.render();
}

void Game::handleEvents()
{
    sf::Event event;
    while (m_windowRef.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_engineRef.stop();
    }
}
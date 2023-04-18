#include "Game.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "core/Engine.hpp"
#include "core/SystemManager.hpp"
#include "core/Log.hpp"

#include "components/Components.hpp"

#include "systems/RenderSystem.hpp"
#include "systems/PlayerSystem.hpp"
#include "systems/InteractableWithPlanetSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/BulletSystem.hpp"

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
    resourceManager.loadTexture("res/textures/01.png", "bullet");

    m_systemManager.addSystem(std::make_unique<PlayerSystem>());
    m_systemManager.addSystem(std::make_unique<InteractableWithPlanetSystem>());
    m_systemManager.addSystem(std::make_unique<PhysicsSystem>());
    m_systemManager.addSystem(std::make_unique<BulletSystem>());

    m_systemManager.addRenderSystem(std::make_unique<RenderSystem>(m_windowRef));

    m_systemManager.init();

    auto& registry = m_systemManager.getRegistry();

    auto planet = registry.create();
    {
        auto& transform = registry.emplace<Transform>(planet);
        transform.position = sf::Vector2f(600.f, 484.f);
        auto& renderable = registry.emplace<Renderable>(planet);
        renderable.sprite.setTexture(resourceManager.getTexture("earth"));
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
        auto& collidable = registry.emplace<Collidable>(planet);
        collidable.radius = renderable.sprite.getLocalBounds().width / 2.f;
    }
    {
        auto player = registry.create();
        auto& playerComponent = registry.emplace<Player>(player);
        playerComponent.speed = 5.f;
        registry.emplace<Body>(player);
        registry.emplace<Transform>(player);
        auto& renderable = registry.emplace<Renderable>(player);
        renderable.sprite.setTexture(resourceManager.getTexture("player_front"));
        renderable.sprite.setPosition(sf::Vector2f(0.f, 0.f));
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
        auto& collidable = registry.emplace<Collidable>(player);
        collidable.radius = renderable.sprite.getLocalBounds().height / 2.f;
        auto& interactableWithPlanet = registry.emplace<InteractableWithPlanet>(player);
        interactableWithPlanet.planet = planet;
        interactableWithPlanet.gravityKoef = 50000000.f;
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
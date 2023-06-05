#include "Game.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "core/Engine.hpp"
#include "core/SystemManager.hpp"
#include "core/Log.hpp"

#include "components/Components.hpp"
#include "systems/Systems.hpp"

#include "EntityType.hpp"

Game::Game(Engine& _engineRef)
    : m_engineRef(_engineRef)
    , m_windowRef(_engineRef.getWindow())
    , m_systemManager(_engineRef)
{
}

void Game::init()
{
    // TODO use the entt hash string 
    auto& resourceManager = m_engineRef.getResourceManager();
    resourceManager.loadTexture("res/textures/earth.png", "earth", TextureConfig{.isSmooth = true});
    resourceManager.loadTexture("res/textures/player_front.png", "player_front", TextureConfig{.isSmooth = true});
    resourceManager.loadTexture("res/textures/01.png", "bullet", TextureConfig{.isSmooth = true});
    resourceManager.loadTexture("res/textures/asteroid.png", "asteroid", TextureConfig{.isSmooth = true});

    resourceManager.loadFont("res/fonts/times_new_roman.ttf", "times_new_roman");

    m_systemManager.addSystem(std::make_unique<PlayerSystem>());
    m_systemManager.addSystem(std::make_unique<InteractableWithPlanetSystem>());
    m_systemManager.addSystem(std::make_unique<GravitySystem>());
    {
        PhysicsSystem::NotificationPairs notificationPairs{{EntityType::ASTEROID, EntityType::BULLET}};
        m_systemManager.addSystem(std::make_unique<PhysicsSystem>(move(notificationPairs)));
    }
    m_systemManager.addSystem(std::make_unique<BulletSystem>());
    m_systemManager.addSystem(std::make_unique<CameraSystem>());
    m_systemManager.addSystem(std::make_unique<AsteroidSystem>());

    m_systemManager.addRenderSystem(std::make_unique<RenderSystem>(m_windowRef));
    m_systemManager.addRenderSystem(std::make_unique<RenderDebugSystem>(m_windowRef));
    m_systemManager.addRenderSystem(std::make_unique<UISystem>(m_windowRef));

    m_systemManager.init();

    auto& registry = m_systemManager.getRegistry();

    auto planet = registry.create();
    registry.ctx().emplace<entt::entity>(planet);   // TODO give a name
    {
        auto& transform = registry.emplace<Transform>(planet);
        transform.position = sf::Vector2f(0.f, 0.f);
     //   transform.scale = 5.f;
        auto& renderable = registry.emplace<Renderable>(planet);
        renderable.sprite.setTexture(resourceManager.getTexture("earth"));
        renderable.sprite.setPosition(transform.position);
     //   renderable.sprite.setScale({5.f, 5.f});
         renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
        auto& collidable = registry.emplace<Collidable>(planet);
        collidable.radius = renderable.sprite.getGlobalBounds().width / 2.f;
        collidable.typeFlag = EntityType::PLANET;
        collidable.canColideWithFlags = EntityType::PLAYER | EntityType::BULLET | EntityType::ASTEROID;
        auto& uiMap = registry.emplace<UIMapComponent>(planet);
        uiMap.color = sf::Color::Blue;
        uiMap.radius = renderable.sprite.getGlobalBounds().height / 2.f;
    }
    auto player = registry.create();
    {
        auto& playerComponent = registry.emplace<Player>(player);
        playerComponent.speed = 5.f;
        playerComponent.cooldownS = 0.5f;
        registry.emplace<Body>(player);
        auto& transform = registry.emplace<Transform>(player);
        transform.position = sf::Vector2f(600.f, 484.f);
        auto& renderable = registry.emplace<Renderable>(player);
        renderable.sprite.setTexture(resourceManager.getTexture("player_front"));
        renderable.sprite.setPosition(sf::Vector2f(0.f, 0.f));
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
      //  renderable.sprite.setScale(0.1f, 0.1f);
        auto& collidable = registry.emplace<Collidable>(player);
        collidable.radius = renderable.sprite.getGlobalBounds().height / 2.f;
        auto& gravity = registry.emplace<Gravity>(player);
        gravity.planet = planet;
        gravity.gravityKoef = 50000000.f;
        auto& interactableWithPlanet = registry.emplace<InteractableWithPlanet>(player);
        interactableWithPlanet.planet = planet;
        collidable.typeFlag = EntityType::PLAYER;
        collidable.canColideWithFlags = EntityType::PLANET | EntityType::ASTEROID;
        auto& uiMap = registry.emplace<UIMapComponent>(player);
        uiMap.color = sf::Color::Green;
        uiMap.radius = renderable.sprite.getGlobalBounds().height / 2.f;
    }
    {
        auto camera = registry.create();
        registry.emplace<Transform>(camera);
        auto& cameraComp = registry.emplace<Camera>(camera);
        cameraComp.target = player;
        cameraComp.size = sf::Vector2f{m_windowRef.getSize().x * 1.5f, m_windowRef.getSize().y * 1.5f};
        cameraComp.offsetFromThePlanet = 200.f;
        cameraComp.speed = 7.f;
        cameraComp.angularSpeed = 7.f;
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
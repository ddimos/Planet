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
    resourceManager.loadTexture("res/textures/spr_missile_half.png", "missile", TextureConfig{.isSmooth = true});
    resourceManager.loadTexture("res/textures/io.png", "satellite", TextureConfig{.isSmooth = true});

    resourceManager.loadFont("res/fonts/times_new_roman.ttf", "times_new_roman");

    m_systemManager.addSystem(std::make_unique<PlayerSystem>());
    m_systemManager.addSystem(std::make_unique<MovableOnPlanetSystem>());
    m_systemManager.addSystem(std::make_unique<GravitySystem>());
    {
        PhysicsSystem::NotificationPairs notificationPairs{
            {EntityType::ASTEROID, EntityType::PROJECTILE},
            {EntityType::PLANET, EntityType::TELEPORT_SPHERE}
        };
        m_systemManager.addSystem(std::make_unique<PhysicsSystem>(move(notificationPairs)));
    }
    m_systemManager.addSystem(std::make_unique<InteractionSystem>());
    m_systemManager.addSystem(std::make_unique<OrbitSystem>());
    m_systemManager.addSystem(std::make_unique<RotationSystem>());
    m_systemManager.addSystem(std::make_unique<ChildSystem>());
    m_systemManager.addSystem(std::make_unique<LifetimeSystem>());
    m_systemManager.addSystem(std::make_unique<DamageSystem>());
    m_systemManager.addSystem(std::make_unique<TargetingSystem>());
    m_systemManager.addSystem(std::make_unique<CameraSystem>());
    m_systemManager.addSystem(std::make_unique<AsteroidSystem>());

    m_systemManager.addRenderSystem(std::make_unique<RenderSystem>(m_windowRef));
    m_systemManager.addRenderSystem(std::make_unique<RenderDebugSystem>(m_windowRef));
    m_systemManager.addRenderSystem(std::make_unique<UISystem>(m_windowRef));

    m_systemManager.init();

    auto& registry = m_systemManager.getRegistry();

    using namespace entt::literals;

    auto planet = registry.create();
    registry.ctx().emplace_as<entt::entity>("planet"_hs, planet);
    {
        auto& entityComp = registry.emplace<EntityComponent>(planet);
        entityComp.type = EntityType::PLANET;

        auto& transform = registry.emplace<Transform>(planet);
        transform.position = sf::Vector2f(0.f, 0.f);
        transform.scale = 3.f;

        auto& renderable = registry.emplace<Renderable>(planet);
        renderable.sprite.setTexture(resourceManager.getTexture("earth"));
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setScale(transform.scale, transform.scale);
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
        
        auto& collidable = registry.emplace<Collidable>(planet);
        collidable.radius = renderable.sprite.getGlobalBounds().width / 2.f;
        collidable.typeFlag = entityComp.type;
        collidable.canColideWithFlags = EntityType::PLAYER | EntityType::PROJECTILE | EntityType::ASTEROID | EntityType::TELEPORT_SPHERE;
        collidable.needToResolve = false;

        auto& interactable = registry.emplace<Interactable>(planet);
        interactable.typeFlag = entityComp.type;
        interactable.canInteractWithFlags = EntityType::TELEPORT_SPHERE;

        auto& planetComp = registry.emplace<Planet>(planet);
        planetComp.gravity = 500.f; // it's not used now
        planetComp.speedMultiplier = 1.f;

        auto& uiMap = registry.emplace<UIMapComponent>(planet);
        uiMap.color = sf::Color::Blue;
        uiMap.radius = renderable.sprite.getGlobalBounds().height / 2.f;

        auto& rotationComponent = registry.emplace<Rotation>(planet);
        rotationComponent.speed = -2.f;
    }

    {
        auto planetAtmosphere = registry.create();
        
        auto& entityComp = registry.emplace<EntityComponent>(planetAtmosphere);
        entityComp.type = EntityType::ATMOSHPERE;

        auto& transform = registry.emplace<Transform>(planetAtmosphere);
        transform.position = sf::Vector2f(0.f, 0.f);
        transform.scale = 3.5f;

        auto& childOfEntity = registry.emplace<ChildOfEntity>(planetAtmosphere);
        childOfEntity.parent = planet;

        auto& renderable = registry.emplace<Renderable>(planetAtmosphere);
        renderable.sprite.setTexture(resourceManager.getTexture("earth"));
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setScale(transform.scale, transform.scale);
        renderable.sprite.setColor(sf::Color{255,255,255, 100});
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
         
        auto& collidable = registry.emplace<Collidable>(planetAtmosphere);
        collidable.radius = renderable.sprite.getGlobalBounds().width / 2.f;
        collidable.typeFlag = entityComp.type;
        collidable.canColideWithFlags = EntityType::TELEPORT_SPHERE;
        collidable.needToResolve = false;
    }

    auto satellite = registry.create();
    {
        auto& entityComp = registry.emplace<EntityComponent>(satellite);
        entityComp.type = EntityType::PLANET;

        auto& planetComp = registry.emplace<Planet>(satellite);
        planetComp.speedMultiplier = 2.f;

        auto& transform = registry.emplace<Transform>(satellite);
        transform.position = sf::Vector2f(1000.f, 1000.f);
        transform.scale = 0.5f;

        auto& renderable = registry.emplace<Renderable>(satellite);
        renderable.sprite.setTexture(resourceManager.getTexture("satellite"));
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setScale(transform.scale, transform.scale);
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
        
        auto& collidable = registry.emplace<Collidable>(satellite);
        collidable.radius = renderable.sprite.getGlobalBounds().width / 2.f;
        collidable.typeFlag = entityComp.type;
        collidable.canColideWithFlags = EntityType::PLAYER | EntityType::PROJECTILE | EntityType::ASTEROID | EntityType::TELEPORT_SPHERE;
        collidable.needToResolve = false;

        auto& interactable = registry.emplace<Interactable>(satellite);
        interactable.typeFlag = entityComp.type;
        interactable.canInteractWithFlags = EntityType::TELEPORT_SPHERE;

        auto& uiMap = registry.emplace<UIMapComponent>(satellite);
        uiMap.color = sf::Color::Cyan;
        uiMap.radius = renderable.sprite.getGlobalBounds().height / 2.f;

        auto& orbit = registry.emplace<Orbit>(satellite);
        orbit.speed = 6.f;

        auto& rotationComponent = registry.emplace<Rotation>(satellite);
        rotationComponent.speed = 8.f;
    }

    {
        auto satelliteAtmosphere = registry.create();
        
        auto& entityComp = registry.emplace<EntityComponent>(satelliteAtmosphere);
        entityComp.type = EntityType::ATMOSHPERE;
        
        auto& transform = registry.emplace<Transform>(satelliteAtmosphere);
        transform.position = sf::Vector2f(0.f, 0.f);
        transform.scale = 0.7f;

        auto& childOfEntity = registry.emplace<ChildOfEntity>(satelliteAtmosphere);
        childOfEntity.parent = satellite;

        auto& renderable = registry.emplace<Renderable>(satelliteAtmosphere);
        renderable.sprite.setTexture(resourceManager.getTexture("satellite"));
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setScale(transform.scale, transform.scale);
        renderable.sprite.setColor(sf::Color{255,255,255, 100});
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
     }

    auto player = registry.create();
    registry.ctx().emplace_as<entt::entity>("player"_hs, player);
    {
        auto& entityComp = registry.emplace<EntityComponent>(player);
        entityComp.type = EntityType::PLAYER;

        auto& playerComponent = registry.emplace<Player>(player);
        playerComponent.speed = 30.f;
        playerComponent.bulletCooldownS = 0.5f;
        playerComponent.missileCooldownS = 5.f;
        playerComponent.speedBoostCoefficient = 2.f;
        playerComponent.speedBoostTimeIntervalS = 1.f;
        playerComponent.speedBoostTimeIntervalDt = playerComponent.speedBoostTimeIntervalS;

   //     registry.emplace<Body>(player);
        
        auto& transform = registry.emplace<Transform>(player);
        transform.position = sf::Vector2f(600.f, 484.f);

        auto& renderable = registry.emplace<Renderable>(player);
        renderable.sprite.setTexture(resourceManager.getTexture("player_front"));
        renderable.sprite.setPosition(sf::Vector2f(0.f, 0.f));
        renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
      //  renderable.sprite.setScale(0.1f, 0.1f);
        
        auto& collidable = registry.emplace<Collidable>(player);
        collidable.radius = renderable.sprite.getGlobalBounds().height / 2.f;
        collidable.typeFlag = entityComp.type;
        collidable.canColideWithFlags = EntityType::PLANET | EntityType::ASTEROID;
        
        auto& movableOnPlanet = registry.emplace<MovableOnPlanet>(player);
        movableOnPlanet.planet = satellite;
        
        auto& uiMap = registry.emplace<UIMapComponent>(player);
        uiMap.color = sf::Color::Green;
        uiMap.radius = renderable.sprite.getGlobalBounds().height / 2.f;
    }

    {
        auto playerTeleportSphere = registry.create();
        
        auto& entityComp = registry.emplace<EntityComponent>(playerTeleportSphere);
        entityComp.type = EntityType::TELEPORT_SPHERE;
        
        auto& transform = registry.emplace<Transform>(playerTeleportSphere);
        transform.position = sf::Vector2f(0.f, 0.f);
        transform.scale = 3.5f;

        auto& childOfEntity = registry.emplace<ChildOfEntity>(playerTeleportSphere);
        childOfEntity.offset = sf::Vector2f{0.f, -200.f};
        childOfEntity.parent = player;

        // auto& renderable = registry.emplace<Renderable>(playerTeleportSphere);

        auto& collidable = registry.emplace<Collidable>(playerTeleportSphere);
        collidable.radius = 170.f;
        collidable.typeFlag = entityComp.type;
        collidable.canColideWithFlags = EntityType::PLANET;
        collidable.needToResolve = false;

        auto& interactable = registry.emplace<Interactable>(playerTeleportSphere);
        interactable.typeFlag = entityComp.type;
        interactable.canInteractWithFlags = EntityType::PLANET;
    }

    {
        auto camera = registry.create();
        
        auto& entityComp = registry.emplace<EntityComponent>(camera);
        entityComp.type = EntityType::CAMERA;

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
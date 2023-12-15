#include "systems/PlayerSystem.hpp"
#include "components/Components.hpp"
#include "events/Events.hpp"

#include "core/Utils.hpp"
#include "core/Engine.hpp"
#include "EntityType.hpp"
#include "core/Log.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace
{
struct Inputs
{
    bool moveLeft = false;
    bool moveRight = false;
    bool boost = false;
    bool shootBullet = false;
    bool launchMissile = false;
    bool teleport = false;
};
}

PlayerSystem::PlayerSystem()
{
}

void PlayerSystem::onInit()
{
    m_dispatcherRef->sink<PlayerShootBulletEvent>().connect<&PlayerSystem::receivePlayerShootBulletEvent>(this);
    m_dispatcherRef->sink<PlayerLaunchMissileEvent>().connect<&PlayerSystem::receivePlayerLaunchMissileEvent>(this);
    m_dispatcherRef->sink<InteractionEvent>().connect<&PlayerSystem::receiveInteractionEvent>(this);
}

void PlayerSystem::onUpdate(float _dt)
{
    using namespace entt::literals;

    Inputs inputs;
    inputs.moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    inputs.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    inputs.boost = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    inputs.shootBullet = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    inputs.launchMissile = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        inputs.teleport = !m_wasTeleportButtonPressedPrevFrame;
        m_wasTeleportButtonPressedPrevFrame = true;
    }
    else
    {
        m_wasTeleportButtonPressedPrevFrame = false;
    }
    

    entt::entity player = m_registryRef->ctx().get<entt::entity>("player"_hs);

    auto& movableOnPlanet = m_registryRef->get<MovableOnPlanet>(player);
    auto& planetTrans = m_registryRef->get<Transform>(movableOnPlanet.planet);
    auto& planetComp = m_registryRef->get<Planet>(movableOnPlanet.planet);
    auto& planetCollidable = m_registryRef->get<Collidable>(movableOnPlanet.planet);
    auto& playerComp = m_registryRef->get<Player>(player);
    auto& playerTransform = m_registryRef->get<Transform>(player);

    //movableOnPlanet.polarPosition;
     
//   if (polarPos.r <= planetCollidable.radius+20)
       movableOnPlanet.polarPosition.r = planetCollidable.radius+40;

    sf::Vector2f deltaPos = {0, 0};
    if (inputs.moveLeft)
        deltaPos.x -= 1.f;
    if (inputs.moveRight)
        deltaPos.x += 1.f;

    float speed = playerComp.speed * planetComp.speedMultiplier;
    if (inputs.boost)
    {
        if (playerComp.speedBoostTimeIntervalDt > 0.f)
        {
            playerComp.speedBoostTimeIntervalDt -= _dt;
            speed *= playerComp.speedBoostCoefficient;
        }
    }
    else
    {
        if (playerComp.speedBoostTimeIntervalDt <= playerComp.speedBoostTimeIntervalS)
            playerComp.speedBoostTimeIntervalDt += _dt;
    }

    movableOnPlanet.polarPosition.phi += deltaPos.x * speed * _dt;

    sf::Vector2f playerRelativePos2 = convertToCartesian(movableOnPlanet.polarPosition);
    sf::Vector2f transfoo2 = rotateVector(playerRelativePos2, (planetTrans.rotation));
    playerTransform.position = planetTrans.position + transfoo2;

    auto normalVec = normalizedVector(planetTrans.position - playerTransform.position);
    auto tangentVec = tangentVector(normalVec);

    if (inputs.shootBullet)
    {
        if (playerComp.bulletCooldownDt <= 0.f)
        {
            playerComp.bulletCooldownDt = playerComp.bulletCooldownS;
            m_dispatcherRef->trigger<PlayerShootBulletEvent>({playerTransform.position, normalVec});
        }
    }
    playerComp.bulletCooldownDt -= _dt;

    if (inputs.launchMissile)
    {
        if (playerComp.missileCooldownDt <= 0.f)
        {
            playerComp.missileCooldownDt = playerComp.missileCooldownS;
            m_dispatcherRef->trigger<PlayerLaunchMissileEvent>({playerTransform.position, normalVec});
        }
    }
    playerComp.missileCooldownDt -= _dt;

    if (inputs.teleport)
    {
        if (m_registryRef->valid(playerComp.canTeleportToPlanet))
        {
            auto& planetTransform = m_registryRef->get<Transform>(playerComp.canTeleportToPlanet);
            auto& planetCollidablee = m_registryRef->get<Collidable>(playerComp.canTeleportToPlanet);
            sf::Vector2f fromPlanetToPlayer = playerTransform.position - planetTransform.position; 
            float anglePositionOnThePlanet = angleBetweenVectors(sf::Vector2f(1.f, 0.f), fromPlanetToPlayer);
            // Technically, the interaction didn't stop immediately after teleporting,
            // so we won't get a new event. We have to change it manually to be able to teleport back.
            std::swap(movableOnPlanet.planet, playerComp.canTeleportToPlanet);
            movableOnPlanet.polarPosition.phi = toDeg(anglePositionOnThePlanet) - planetTransform.rotation;
        }  
    }
}

void PlayerSystem::receivePlayerShootBulletEvent(const PlayerShootBulletEvent& _event)
{
    auto bullet = m_registryRef->create();

    auto& entityComp = m_registryRef->emplace<EntityComponent>(bullet);
    entityComp.type = EntityType::PROJECTILE;

    auto& shootableComp = m_registryRef->emplace<Shootable>(bullet);
    shootableComp.speed = 250.f;
    shootableComp.canHaveTarget = false;

    auto& lifetimeComp = m_registryRef->emplace<LifetimeComponent>(bullet);
    lifetimeComp.lifetimeS = 3.0f;

    auto& body = m_registryRef->emplace<Body>(bullet);
    body.velocity = -shootableComp.speed * _event.direction;

    auto& transform = m_registryRef->emplace<Transform>(bullet);
    transform.rotation = -toDeg(angleBetweenVectors(_event.direction, sf::Vector2f(-1.f, 0.f)));
    transform.position = _event.position;

    auto& renderable = m_registryRef->emplace<Renderable>(bullet);
    renderable.sprite.setTexture(m_engineRef->getResourceManager().getTexture("bullet"));
    renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
    
    auto& collidable = m_registryRef->emplace<Collidable>(bullet);
    collidable.radius = renderable.sprite.getGlobalBounds().height / 6.f;
    collidable.typeFlag = entityComp.type;
    collidable.canColideWithFlags = EntityType::ASTEROID | EntityType::PLANET;

    auto& uiMap = m_registryRef->emplace<UIMapComponent>(bullet);
    uiMap.color = sf::Color::White;
    uiMap.radius = renderable.sprite.getGlobalBounds().height / 4.f;
}

void PlayerSystem::receivePlayerLaunchMissileEvent(const PlayerLaunchMissileEvent& _event)
{
    auto missile = m_registryRef->create();

    auto& entityComp = m_registryRef->emplace<EntityComponent>(missile);
    entityComp.type = EntityType::PROJECTILE;

    auto& shootableComp = m_registryRef->emplace<Shootable>(missile);
    shootableComp.speed = 200.f;
    shootableComp.canHaveTarget = true;

    auto& lifetimeComp = m_registryRef->emplace<LifetimeComponent>(missile);
    lifetimeComp.lifetimeS = 6.0f;

    auto& body = m_registryRef->emplace<Body>(missile);
    body.velocity = -shootableComp.speed * _event.direction;

    auto& transform = m_registryRef->emplace<Transform>(missile);
    transform.rotation = -toDeg(angleBetweenVectors(_event.direction, sf::Vector2f(-1.f, 0.f)));
    transform.position = _event.position;

    auto& renderable = m_registryRef->emplace<Renderable>(missile);
    renderable.sprite.setTexture(m_engineRef->getResourceManager().getTexture("missile"));
    renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);

    auto& collidable = m_registryRef->emplace<Collidable>(missile);
    collidable.radius = renderable.sprite.getGlobalBounds().height / 6.f;
    collidable.typeFlag = entityComp.type;
    collidable.canColideWithFlags = EntityType::ASTEROID | EntityType::PLANET;

    auto& uiMap = m_registryRef->emplace<UIMapComponent>(missile);
    uiMap.color = sf::Color::White;
    uiMap.radius = renderable.sprite.getGlobalBounds().height / 6.f;
}

void PlayerSystem::receiveInteractionEvent(const InteractionEvent& _event)
{
    EntityType typeA = m_registryRef->get<EntityComponent>(_event.entityA).type;
    EntityType typeB = m_registryRef->get<EntityComponent>(_event.entityB).type;
    
    auto setPlayerCanTeleport = [this](entt::entity _teleportSphere, entt::entity _planet, bool _interactionStarted) {
        auto& childTeleport = m_registryRef->get<ChildOfEntity>(_teleportSphere);
        
        auto& player = m_registryRef->get<Player>(childTeleport.parent);
        auto currentPlanet = m_registryRef->get<MovableOnPlanet>(childTeleport.parent).planet;
        if (currentPlanet != _planet)
            player.canTeleportToPlanet = _interactionStarted ? _planet : entt::null;
    };


    if (typeA == EntityType::PLANET && typeB == EntityType::TELEPORT_SPHERE)
    {
        setPlayerCanTeleport(_event.entityB, _event.entityA, _event.state == InteractionEvent::InteractionState::STARTED);
    }
    else if (typeA == EntityType::TELEPORT_SPHERE && typeB == EntityType::PLANET)
    {
        setPlayerCanTeleport(_event.entityA, _event.entityB, _event.state == InteractionEvent::InteractionState::STARTED);
    }
}

#include "systems/PlayerSystem.hpp"
#include "components/Components.hpp"
#include "events/Events.hpp"

#include "core/Utils.hpp"
#include "core/Engine.hpp"
#include "EntityType.hpp"

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
};
}

PlayerSystem::PlayerSystem()
{
}

void PlayerSystem::onInit()
{
    m_dispatcherRef->sink<PlayerShootBulletEvent>().connect<&PlayerSystem::receivePlayerShootBulletEvent>(this);
    m_dispatcherRef->sink<PlayerLaunchMissileEvent>().connect<&PlayerSystem::receivePlayerLaunchMissileEvent>(this);
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
    
    entt::entity player = m_registryRef->ctx().get<entt::entity>("player"_hs);

    auto& playerComp = m_registryRef->get<Player>(player);
    auto& playerTransform = m_registryRef->get<Transform>(player);
    auto polarPos = convertToPolar(playerTransform.position);
    
    sf::Vector2f deltaPos = {0, 0};
    if (inputs.moveLeft)
        deltaPos.x -= 1.f;
    if (inputs.moveRight)
        deltaPos.x += 1.f;

    float speed = playerComp.speed;
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

    polarPos.phi += deltaPos.x * speed * _dt;
    playerTransform.position = convertToCartesian(polarPos);

    auto& planet = m_registryRef->get<InteractableWithPlanet>(player);
    auto& planetTrans = m_registryRef->get<Transform>(planet.planet);
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
}

void PlayerSystem::receivePlayerShootBulletEvent(const PlayerShootBulletEvent& _event)
{
    auto bullet = m_registryRef->create();

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
    collidable.typeFlag = EntityType::PROJECTILE;
    collidable.canColideWithFlags = EntityType::ASTEROID | EntityType::PLANET;
    auto& uiMap = m_registryRef->emplace<UIMapComponent>(bullet);
    uiMap.color = sf::Color::White;
    uiMap.radius = renderable.sprite.getGlobalBounds().height / 4.f;
}

void PlayerSystem::receivePlayerLaunchMissileEvent(const PlayerLaunchMissileEvent& _event)
{
    auto missile = m_registryRef->create();

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
    collidable.typeFlag = EntityType::PROJECTILE;
    collidable.canColideWithFlags = EntityType::ASTEROID | EntityType::PLANET;
    auto& uiMap = m_registryRef->emplace<UIMapComponent>(missile);
    uiMap.color = sf::Color::White;
    uiMap.radius = renderable.sprite.getGlobalBounds().height / 6.f;
}
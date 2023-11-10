#include "systems/PlayerSystem.hpp"
#include "components/Components.hpp"
#include "events/Events.hpp"

#include "core/Utils.hpp"
#include "core/Engine.hpp"
#include "EntityType.hpp"

#include <SFML/Window/Keyboard.hpp>

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
    float m_phi = 0;
    float m_r = 0;
    auto view = m_registryRef->view<Body, Transform, Player>();
    for(auto entity : view)
    {
        auto& player = view.get<Player>(entity);
        auto& body = view.get<Body>(entity);
        auto& transform = view.get<Transform>(entity);

        sf::Vector2f deltaPos = {0, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            deltaPos.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            deltaPos.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            deltaPos.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            deltaPos.y += 1.f;
        auto& planet = m_registryRef->get<InteractableWithPlanet>(entity);
        auto& planetTrans = m_registryRef->get<Transform>(planet.planet);
        auto normalVec = normalizedVector(planetTrans.position - transform.position);
        auto tangentVec = tangentVector(normalVec);

        body.velocity += player.speed * deltaPos.x * tangentVec;
        body.velocity += player.speed * deltaPos.y * normalVec;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (player.bulletCooldownDt <= 0.f)
            {
                player.bulletCooldownDt = player.bulletCooldownS;
                m_dispatcherRef->trigger<PlayerShootBulletEvent>({transform.position, normalVec});
            }
        }
        player.bulletCooldownDt -= _dt;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            if (player.missileCooldownDt <= 0.f)
            {
                player.missileCooldownDt = player.missileCooldownS;
                m_dispatcherRef->trigger<PlayerLaunchMissileEvent>({transform.position, normalVec});
            }
        }
        player.missileCooldownDt -= _dt;
        
    }
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
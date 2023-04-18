#include "systems/BulletSystem.hpp"
#include "events/Events.hpp"
#include "components/Components.hpp"
#include "core/Engine.hpp"
#include "core/Utils.hpp"

void BulletSystem::onInit()
{
    m_dispatcherRef->sink<ShootEvent>().connect<&BulletSystem::receiveShootEvent>(this);
    m_dispatcherRef->sink<CollisionEvent>().connect<&BulletSystem::receiveCollisionEvent>(this);
}

void BulletSystem::onUpdate(float _dt)
{
    for(auto &&[entity, bullet] : m_registryRef->view<Bullet>().each())
    {
        bullet.lifetimeS -= _dt;
        if (bullet.lifetimeS <= 0.f)
        {
            m_registryRef->destroy(entity);
        }
    }
}

void BulletSystem::receiveShootEvent(const ShootEvent& _event)
{
    auto bullet = m_registryRef->create();

    auto& bulletComp = m_registryRef->emplace<Bullet>(bullet);
    bulletComp.lifetimeS = 3.0f;
    auto& body = m_registryRef->emplace<Body>(bullet);
    body.velocity = -100.f * _event.direction;
    auto& transform = m_registryRef->emplace<Transform>(bullet);
    transform.rotation = -toDeg(angleBetweenVectors(_event.direction, sf::Vector2f(-1.f, 0.f)));
    transform.position = _event.position;
    auto& renderable = m_registryRef->emplace<Renderable>(bullet);
    renderable.sprite.setTexture(m_engineRef->getResourceManager().getTexture("bullet"));
    renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
    // auto& collidable = m_registryRef->emplace<Collidable>(bullet);
    // collidable.radius = renderable.sprite.getLocalBounds().height / 2.f;
    // auto& interactableWithPlanet = m_registryRef->emplace<InteractableWithPlanet>(bullet);
    // interactableWithPlanet.planet = planet;
    // interactableWithPlanet.gravityKoef = 10000000.f;
}

void BulletSystem::receiveCollisionEvent(const CollisionEvent& _event)
{

}

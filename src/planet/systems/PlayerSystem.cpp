#include "systems/PlayerSystem.hpp"
#include "components/Components.hpp"
#include "events/Events.hpp"
#include "core/Utils.hpp"
#include <SFML/Window/Keyboard.hpp>

PlayerSystem::PlayerSystem()
{
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
            if (player.cooldownDt <= 0.f)
            {
                player.cooldownDt = player.cooldownS;
                m_dispatcherRef->trigger<ShootEvent>({transform.position, normalVec});
            }
        }
        player.cooldownDt -= _dt;
    }
}

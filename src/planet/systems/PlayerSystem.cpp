#include "systems/PlayerSystem.hpp"
#include "components/Components.hpp"
#include <SFML/Window/Keyboard.hpp>

PlayerSystem::PlayerSystem()
{
}

void PlayerSystem::onUpdate(float _dt)
{
    float m_phi = 0;
    float m_r = 0;
    auto view = m_registryRef->view<Body, Player>();
    for(auto entity : view)
    {
        auto& player = view.get<Player>(entity);
        auto& body = view.get<Body>(entity);
        sf::Vector2f deltaPos = {0, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            deltaPos.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            deltaPos.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            deltaPos.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            deltaPos.y += 1.f;

        body.velocity += player.speed * deltaPos;
    }
}

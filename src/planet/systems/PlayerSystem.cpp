#include "systems/PlayerSystem.hpp"
#include "components/Components.hpp"
#include <SFML/Window/Keyboard.hpp>

PlayerSystem::PlayerSystem()
{
}

void PlayerSystem::onUpdate(float _dt)
{
    auto view = m_registryRef->view<Transform, Player>();
    const float speed = 50.f;
    for(auto entity : view)
    {
        auto& transform = view.get<Transform>(entity);
        sf::Vector2i deltaPos = {0, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            deltaPos.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            deltaPos.x += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            deltaPos.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            deltaPos.y += 1;

        transform.position.x += speed * _dt * deltaPos.x;
        transform.position.y += speed * _dt * deltaPos.y;
    }
}

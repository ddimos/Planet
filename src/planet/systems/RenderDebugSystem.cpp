#include "systems/RenderDebugSystem.hpp"
#include "components/Components.hpp"

#include <SFML/Graphics.hpp>

// TODO ifdef 
    
RenderDebugSystem::RenderDebugSystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
    , m_isCollidersEnabled(true)
{
}

void RenderDebugSystem::onUpdate(float _dt)
{
    if (!m_isCollidersEnabled)
        return;

    for(auto &&[entity, transform, collidable] : m_registryRef->view<Transform, Collidable>().each())
    {
        sf::CircleShape shape;
        shape.setPosition(transform.position);
        shape.setOrigin(collidable.radius, collidable.radius);
        shape.setRadius(collidable.radius);
        shape.setOutlineColor(sf::Color::Red);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(3);

        m_windowRef.draw(shape);
    }
}

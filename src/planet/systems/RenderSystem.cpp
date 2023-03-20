#include "systems/RenderSystem.hpp"
#include "components/Components.hpp"

#include <SFML/Graphics.hpp>

RenderSystem::RenderSystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
{
}

void RenderSystem::onUpdate(float _dt)
{
    for(auto &&[entity, transform, renderable] : m_registryRef->view<Transform, Renderable>().each())
    {
        renderable.sprite.setPosition(transform.position);
        m_windowRef.draw(renderable.sprite);
    }
}
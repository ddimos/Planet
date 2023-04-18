#include "systems/RenderSystem.hpp"
#include "components/Components.hpp"
#include "core/Engine.hpp"

#include <SFML/Graphics.hpp>

RenderSystem::RenderSystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
{
}

void RenderSystem::onInit()
{
    m_background.setTexture(m_engineRef->getResourceManager().getTexture("sky_stars"));
    m_background.setPosition(sf::Vector2f(0.f, 0.f));
}

void RenderSystem::onUpdate(float _dt)
{
    m_windowRef.draw(m_background);
    for(auto &&[entity, transform, renderable] : m_registryRef->view<Transform, Renderable>().each())
    {
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setRotation(transform.rotation);
        m_windowRef.draw(renderable.sprite);
    }
}
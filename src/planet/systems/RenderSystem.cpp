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
    m_background.setPosition(sf::Vector2f(-300.f, -300.f));

    auto windowSize = m_windowRef.getSize();
//    m_view.setSize(windowSize.x*0.3f, windowSize.y*0.3f);
   m_view.setSize(windowSize.x, windowSize.y);
}

void RenderSystem::onUpdate(float _dt)
{
    for(auto &&[entity, transform, camera] : m_registryRef->view<Transform, Camera>().each()) // should be only one camera
    {
        m_view.setCenter(transform.position);
        m_view.setRotation(transform.rotation);
        m_view.setSize(camera.size);
    }

    m_windowRef.setView(m_view);
    m_windowRef.draw(m_background);
    for(auto &&[entity, transform, renderable] : m_registryRef->view<Transform, Renderable>().each())
    {
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setRotation(transform.rotation);
        m_windowRef.draw(renderable.sprite);
    }
}

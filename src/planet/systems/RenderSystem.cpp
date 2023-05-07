#include "systems/RenderSystem.hpp"
#include "components/CameraComponent.hpp"
#include "components/RenderableComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Engine.hpp"

#include <SFML/Graphics.hpp>

RenderSystem::RenderSystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
{
}

void RenderSystem::onInit()
{
    m_background.setTexture(m_engineRef->getResourceManager().getTexture("sky_stars"));
    m_background.setPosition(sf::Vector2f(-2000.f, -1500.f));
    m_background.setTextureRect({0,0, 4000, 3000});

    auto windowSize = m_windowRef.getSize();
    m_view.setSize(windowSize.x, windowSize.y);
}

void RenderSystem::onUpdate(float _dt)
{
    if (auto cameraView = m_registryRef->view<Transform, Camera>())
    {
        if (cameraView.size_hint() > 0)
        {
            auto &&[transform, camera] = cameraView.get<Transform, Camera>(*cameraView.begin());
            m_view.setCenter(transform.position);
            m_view.setRotation(transform.rotation);
            m_view.setSize(camera.size);
        }
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

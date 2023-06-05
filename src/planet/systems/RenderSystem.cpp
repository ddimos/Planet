#include "systems/RenderSystem.hpp"
#include "components/CameraComponent.hpp"
#include "components/RenderableComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Engine.hpp"

#include <SFML/Graphics.hpp>

RenderSystem::RenderSystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
    , m_starfield(_window)
{
}

void RenderSystem::onInit()
{
    auto windowSize = m_windowRef.getSize();
    m_view.setSize(windowSize.x, windowSize.y);

    m_starfield.init();
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
    m_starfield.update(m_view.getCenter());

    for(auto &&[entity, transform, renderable] : m_registryRef->view<Transform, Renderable>().each())
    {
        renderable.sprite.setPosition(transform.position);
        renderable.sprite.setRotation(transform.rotation);
        m_windowRef.draw(renderable.sprite);
    }
}

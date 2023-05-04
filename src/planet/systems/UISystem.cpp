#include "systems/UISystem.hpp"

#include "components/UIMapComponent.hpp"
#include "components/RenderableComponent.hpp"
#include "components/TransformComponent.hpp"
#include <SFML/Graphics.hpp>

#include "EntityType.hpp"

UISystem::UISystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
{
    m_minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.333f));
    m_minimapView.setSize(m_minimapSize);
    m_minimapView.setCenter(0.f, 0.f);
}

void UISystem::onUpdate(float _dt)
{
    const auto& view = m_windowRef.getView();
    m_windowRef.setView(m_minimapView);

    sf::CircleShape shape;
    shape.setPosition(0, 0);
    shape.setOrigin(m_minimapSize / 2.f);
    shape.setRadius(m_minimapSize.x / 2.f);    
    shape.setFillColor(sf::Color{255, 255, 100, 50});

    m_windowRef.draw(shape);

    for(auto &&[entity, transform, mapComponent] : m_registryRef->view<Transform, UIMapComponent>().each())
    {
        sf::CircleShape shape;
        shape.setPosition(transform.position);
        shape.setOrigin(mapComponent.radius, mapComponent.radius);
        shape.setRadius(mapComponent.radius);
        
        shape.setFillColor(mapComponent.color);

        m_windowRef.draw(shape);
    }

    m_windowRef.setView(view);
}

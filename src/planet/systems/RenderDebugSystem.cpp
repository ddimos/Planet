#include "systems/RenderDebugSystem.hpp"
#include "components/BodyComponent.hpp"
#include "components/CameraComponent.hpp"
#include "components/CollidableComponent.hpp"
#include "components/TransformComponent.hpp"
#include <SFML/Graphics.hpp>
#include "core/Engine.hpp"

#include <sstream>

// TODO ifdef 
    
RenderDebugSystem::RenderDebugSystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
    , m_isCollidersEnabled(false)
    , m_isPosAndVelEnabled(false)
{
}

void RenderDebugSystem::onUpdate(float _dt)
{
    if (m_isCollidersEnabled)
    {
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

    if (m_isPosAndVelEnabled)
    {
        float rotation = 0;
        if (auto cameraView = m_registryRef->view<Transform, Camera>())
        {
            if (cameraView.size_hint() > 0)
            {
                auto& transform = cameraView.get<Transform>(*cameraView.begin());
                rotation = transform.rotation;
            }
        }
        const auto& font = m_engineRef->getResourceManager().getFont("times_new_roman");
        for(auto &&[entity, transform, body] : m_registryRef->view<Transform, Body>().each())
        {
            sf::Text text;
            text.setFont(font);
            text.setPosition(transform.position);
            text.setRotation(rotation);
            std::stringstream sstream;
            sstream << (int)transform.position.x << " " << (int)transform.position.y << "\n" << (int)body.velocity.x << " " << (int)body.velocity.y;
            text.setString(sstream.str());
            text.setCharacterSize(15);
            text.setOutlineThickness(1.5f);
            text.setStyle(sf::Text::Bold);
            text.setFillColor(sf::Color::Magenta);
            text.setOutlineColor(sf::Color::Black);

            m_windowRef.draw(text);
        }
    }
}

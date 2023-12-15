#include "systems/UISystem.hpp"

#include "components/PlayerComponent.hpp"
#include "components/RenderableComponent.hpp"
#include "components/TransformComponent.hpp"
#include "components/UIMapComponent.hpp"

#include "core/Engine.hpp"

#include <SFML/Graphics.hpp>

#include "EntityType.hpp"

namespace
{
    const sf::Vector2f SPEED_BOOST_PROGRESS_BAR_SIZE{ 60.f, 30.f };
}

UISystem::UISystem(sf::RenderWindow& _window)
    : m_windowRef(_window)
{
    m_minimapSize *= 2.f;
    m_minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.333f));
    m_minimapView.setSize(m_minimapSize);
    m_minimapView.setCenter(0.f, 0.f);
    // TODO to take into account resolution (1024, 768)
    
    m_canTeleportText.setPosition(sf::Vector2f{860.f, 750.f});
    m_canTeleportText.setCharacterSize(24);
    m_canTeleportText.setString("Teleport");

    m_bulletCooldownText.setPosition(sf::Vector2f{860.f, 800.f});
    m_bulletCooldownText.setCharacterSize(30);

    m_missileCooldownText.setPosition(sf::Vector2f{860.f, 850.f});
    m_missileCooldownText.setCharacterSize(30);

    m_speedBoostProgress.setPosition(sf::Vector2f{860.f, 900.f});
    m_speedBoostProgress.setFillColor(sf::Color::White);
    m_speedBoostProgress.setSize(SPEED_BOOST_PROGRESS_BAR_SIZE);
}

void UISystem::onInit()
{
    const auto& font = m_engineRef->getResourceManager().getFont("times_new_roman");
    m_canTeleportText.setFont(font);
    m_bulletCooldownText.setFont(font);
    m_missileCooldownText.setFont(font);
}

void UISystem::onUpdate(float _dt)
{
    using namespace entt::literals;

    const auto& view = m_windowRef.getView();
    m_windowRef.setView(m_uiView);

    {    
        entt::entity player = m_registryRef->ctx().get<entt::entity>("player"_hs);
        auto& playerComponent = m_registryRef->get<Player>(player);

        if (m_registryRef->valid(playerComponent.canTeleportToPlanet))
            m_windowRef.draw(m_canTeleportText);

        if (playerComponent.bulletCooldownDt <= 0.f)
            m_bulletCooldownText.setString("Fire"); // TODO set it only once
        else
            m_bulletCooldownText.setString(std::to_string(playerComponent.bulletCooldownDt));

        if (playerComponent.missileCooldownDt <= 0.f)
            m_missileCooldownText.setString("Fire");
        else
            m_missileCooldownText.setString(std::to_string(playerComponent.missileCooldownDt));

        m_windowRef.draw(m_bulletCooldownText);
        m_windowRef.draw(m_missileCooldownText);

        if (playerComponent.speedBoostTimeIntervalS > 0.f)
        {
            m_speedBoostProgress.setSize(
                sf::Vector2f(
                    (SPEED_BOOST_PROGRESS_BAR_SIZE.x * playerComponent.speedBoostTimeIntervalDt) 
                            / playerComponent.speedBoostTimeIntervalS,
                     SPEED_BOOST_PROGRESS_BAR_SIZE.y));
            m_windowRef.draw(m_speedBoostProgress);
        }
    }

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

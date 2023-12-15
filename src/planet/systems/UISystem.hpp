#pragma once
#include "systems/BaseSystem.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
    class RenderWindow;
};

class UISystem : public BaseSystem
{
public:
    UISystem(sf::RenderWindow& _window);

private:
    void onInit() override;
    void onUpdate(float _dt) override;

    sf::RenderWindow& m_windowRef;
    sf::View m_minimapView;
    sf::View m_uiView;

    sf::Vector2f m_minimapSize{5000.f, 5000.f};

    sf::Text m_canTeleportText;
    sf::Text m_bulletCooldownText;
    sf::Text m_missileCooldownText;
    sf::RectangleShape m_speedBoostProgress;
};

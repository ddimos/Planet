#pragma once
#include "systems/BaseSystem.hpp"
#include <SFML/Graphics/View.hpp>

namespace sf
{
    class RenderWindow;
};

class UISystem : public BaseSystem
{
public:
    UISystem(sf::RenderWindow& _window);

private:
    void onUpdate(float _dt) override;

    sf::RenderWindow& m_windowRef;
    sf::View m_minimapView;

    sf::Vector2f m_minimapSize{5000.f, 5000.f};
};

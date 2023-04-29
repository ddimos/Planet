#pragma once
#include "systems/BaseSystem.hpp"
#include <SFML/Graphics/Sprite.hpp> // for the background
#include <SFML/Graphics/View.hpp>

namespace sf
{
    class RenderWindow;
};

class RenderSystem : public BaseSystem
{
public:
    RenderSystem(sf::RenderWindow& _window);

private:
    void onUpdate(float _dt) override;
    void onInit() override;

    sf::RenderWindow& m_windowRef;
    sf::Sprite m_background;
    sf::View m_view;
};

#pragma once
#include "systems/BaseSystem.hpp"
#include <SFML/Graphics/View.hpp>
#include "Starfield.h"

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
    sf::View m_view;

    Starfield m_starfield;
};

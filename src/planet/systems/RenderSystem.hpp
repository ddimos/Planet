#pragma once
#include "systems/BaseSystem.hpp"

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

    sf::RenderWindow& m_windowRef;
};
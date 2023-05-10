#pragma once

#include "systems/BaseSystem.hpp"

namespace sf
{
    class RenderWindow;
};

class RenderDebugSystem : public BaseSystem
{
public:
    RenderDebugSystem(sf::RenderWindow& _window);

private:
    void onUpdate(float _dt) override;

    sf::RenderWindow& m_windowRef;
    bool m_isCollidersEnabled = false;
    bool m_isPosAndVelEnabled = false;
};

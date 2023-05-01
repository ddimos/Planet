#pragma once

#include "systems/BaseSystem.hpp"
#include "core/Random.h"

class AsteroidSystem : public BaseSystem
{
public:
    AsteroidSystem();

private:
    void onUpdate(float _dt) override;
    void spawn();

    const float m_spawnFrequencyS = 6.f;
    float m_spawnTimeS = m_spawnFrequencyS;
    bool m_isPressed = false;
    Random m_randomizer;
};

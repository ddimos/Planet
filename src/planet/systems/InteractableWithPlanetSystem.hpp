#pragma once
#include "systems/BaseSystem.hpp"

class InteractableWithPlanetSystem : public BaseSystem
{
public:
    InteractableWithPlanetSystem() = default;

private:
    void onUpdate(float _dt) override;

    const float m_rotSpeed = 100.f;
    const float m_rotThreshold = 1.f;
};

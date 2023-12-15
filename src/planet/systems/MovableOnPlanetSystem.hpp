#pragma once
#include "systems/BaseSystem.hpp"

class MovableOnPlanetSystem : public BaseSystem
{
public:
    MovableOnPlanetSystem() = default;

private:
    void onUpdate(float _dt) override;

};

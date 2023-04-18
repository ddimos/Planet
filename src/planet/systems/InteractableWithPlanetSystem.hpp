#pragma once
#include "systems/BaseSystem.hpp"

class InteractableWithPlanetSystem : public BaseSystem
{
public:
    InteractableWithPlanetSystem() = default;

private:
    void onUpdate(float _dt) override;

};

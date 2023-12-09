#pragma once

#include "systems/BaseSystem.hpp"

class RotationSystem : public BaseSystem
{
public:
    RotationSystem() = default;

private:
    void onUpdate(float _dt) override;
};

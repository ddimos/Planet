#pragma once

#include "systems/BaseSystem.hpp"

class GravitySystem : public BaseSystem
{
public:
    GravitySystem() = default;

private:
    void onUpdate(float _dt) override;
};

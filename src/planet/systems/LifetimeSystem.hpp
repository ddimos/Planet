#pragma once

#include "systems/BaseSystem.hpp"

class LifetimeSystem : public BaseSystem
{
public:
    LifetimeSystem() = default;

private:
    void onUpdate(float _dt) override;
};

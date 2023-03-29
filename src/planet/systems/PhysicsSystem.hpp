#pragma once
#include "systems/BaseSystem.hpp"

class PhysicsSystem : public BaseSystem
{
public:
    PhysicsSystem() = default;

private:
    void onUpdate(float _dt) override;
};

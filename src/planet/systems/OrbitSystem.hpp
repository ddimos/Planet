#pragma once

#include "systems/BaseSystem.hpp"

class OrbitSystem : public BaseSystem
{
public:
    OrbitSystem() = default;

private:
    void onUpdate(float _dt) override;
};

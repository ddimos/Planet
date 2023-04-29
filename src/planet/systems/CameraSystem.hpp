#pragma once
#include "systems/BaseSystem.hpp"

class CameraSystem : public BaseSystem
{
public:
    CameraSystem() = default;

private:
    void onUpdate(float _dt) override;
};

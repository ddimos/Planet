#pragma once
#include "systems/BaseSystem.hpp"


class PlayerSystem : public BaseSystem
{
public:
    PlayerSystem();

private:
    void onUpdate(float _dt) override;
};
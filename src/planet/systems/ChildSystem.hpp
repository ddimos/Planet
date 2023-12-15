#pragma once
#include "systems/BaseSystem.hpp"

class ChildSystem : public BaseSystem
{
public:
    ChildSystem() = default;

private:
    void onUpdate(float _dt) override;
};

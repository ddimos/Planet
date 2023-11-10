#pragma once
#include "systems/BaseSystem.hpp"

class CollisionEvent;
class PlayerShootBulletEvent;

class DamageSystem : public BaseSystem
{
public:
    DamageSystem() = default;

private:
    void onInit() override;

    void receiveCollisionEvent(const CollisionEvent& _event);
};

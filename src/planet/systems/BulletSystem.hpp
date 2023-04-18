#pragma once
#include "systems/BaseSystem.hpp"

class CollisionEvent;
class ShootEvent;

class BulletSystem : public BaseSystem
{
public:
    BulletSystem() = default;

private:
    void onInit() override;
    void onUpdate(float _dt) override;

    void receiveShootEvent(const ShootEvent& _event);
    void receiveCollisionEvent(const CollisionEvent& _event);
};

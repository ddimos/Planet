#pragma once
#include "systems/BaseSystem.hpp"


class PlayerShootBulletEvent;
class PlayerLaunchMissileEvent;

class PlayerSystem : public BaseSystem
{
public:
    PlayerSystem();

private:
    void onInit() override;
    void onUpdate(float _dt) override;

    void receivePlayerShootBulletEvent(const PlayerShootBulletEvent& _event);
    void receivePlayerLaunchMissileEvent(const PlayerLaunchMissileEvent& _event);
};
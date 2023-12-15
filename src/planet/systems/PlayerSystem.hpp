#pragma once
#include "systems/BaseSystem.hpp"


class PlayerShootBulletEvent;
class PlayerLaunchMissileEvent;
class InteractionEvent;

class PlayerSystem : public BaseSystem
{
public:
    PlayerSystem();

private:
    void onInit() override;
    void onUpdate(float _dt) override;

    void receivePlayerShootBulletEvent(const PlayerShootBulletEvent& _event);
    void receivePlayerLaunchMissileEvent(const PlayerLaunchMissileEvent& _event);
    void receiveInteractionEvent(const InteractionEvent& _event);

    bool m_wasTeleportButtonPressedPrevFrame = false;
};
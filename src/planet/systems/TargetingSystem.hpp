#pragma once

#include "systems/BaseSystem.hpp"

#include <SFML/System/Vector2.hpp>

class PlayerLaunchMissile;

class TargetingSystem : public BaseSystem
{
public:
    TargetingSystem() = default;

private:
    void onUpdate(float _dt) override;

    entt::entity defineTarget(sf::Vector2f _position);
};

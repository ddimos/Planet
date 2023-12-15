#pragma once

struct Player
{
    float speed = 0.f;
    float bulletCooldownS = 0.f;
    float missileCooldownS = 0.f;
    float bulletCooldownDt = 0.f;
    float missileCooldownDt = 0.f;
    
    float speedBoostCoefficient = 0.f;
    float speedBoostTimeIntervalS = 0.f;
    float speedBoostTimeIntervalDt = 0.f;

    entt::entity canTeleportToPlanet = entt::null;
};

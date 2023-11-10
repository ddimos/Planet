#pragma once

struct Shootable
{
    float speed = 0.f;
    entt::entity target = entt::null;
    bool canHaveTarget = false;
};

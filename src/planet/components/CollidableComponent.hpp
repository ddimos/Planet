#pragma once
#include <cstdint>

using CollidableType = uint16_t;
struct Collidable
{
    float radius = 0.f;
    CollidableType typeFlag = 0;
    CollidableType canColideWithFlags = 0;
    bool needToResolve = true;
};
#pragma once
#include <cstdint>

enum EntityType : uint16_t
{
    None                = 0u,
    PLAYER              = (1u << 0),
    CAMERA              = (1u << 1),
    PLANET              = (1u << 2),
    PROJECTILE          = (1u << 3),
    ASTEROID            = (1u << 4),
    ATMOSHPERE          = (1u << 5),
    TELEPORT_SPHERE     = (1u << 6),
};

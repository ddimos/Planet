#pragma once
#include <cstdint>

enum EntityType : uint16_t
{
    None                = 0u,
    PLAYER              = (1u << 0),
    PLANET              = (1u << 1),
    BULLET              = (1u << 2),
    ASTEROID            = (1u << 3),
};

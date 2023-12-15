#pragma once
#include "core/Utils.hpp"

struct MovableOnPlanet
{
    entt::entity planet = entt::null;
    PolarPos polarPosition{};
};

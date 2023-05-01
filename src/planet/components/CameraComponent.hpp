#pragma once

#include <SFML/System/Vector2.hpp>

struct Camera
{
    entt::entity target = entt::null;
    sf::Vector2f size = {};
    float offsetFromThePlanet = 0.f;
};

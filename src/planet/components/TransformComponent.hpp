#pragma once
#include <SFML/System/Vector2.hpp>

struct Transform
{
    sf::Vector2f position = sf::Vector2f{0.f, 0.f};
    float rotation = 0.f;
    float scale = 1.f;
};

#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>

struct Transform
{
    sf::Vector2f position = sf::Vector2f{0.f, 0.f};
    sf::Vector2f rotation = sf::Vector2f{0.f, 0.f};
    float scale = 1.f;
};

struct Renderable
{
    sf::Sprite sprite;
};

struct Player
{
};

struct Motion
{
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
};
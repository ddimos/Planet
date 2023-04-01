#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>

struct Transform
{
    sf::Vector2f position = sf::Vector2f{0.f, 0.f};
    float rotation = 0.f;
    float scale = 1.f;
};

struct Renderable
{
    sf::Sprite sprite;
};

struct Player
{
    float speed = 0.f;
};

struct Body
{
    sf::Vector2f velocity;
    sf::Vector2f force;
};

struct Collidable
{
    float radius = 0.f;
};

struct InteractableWithPlanet
{
    entt::entity planet = entt::null;
    float gravityKoef = 0.f;
};

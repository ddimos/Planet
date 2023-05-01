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
    float cooldownS = 0.f;
};

struct Body
{
    sf::Vector2f velocity;
    sf::Vector2f force;
};

using CollidableType = uint16_t;
struct Collidable
{
    float radius = 0.f;
    CollidableType typeFlag = 0;
    CollidableType canColideWithFlags = 0;
};

struct Gravity
{
    float gravityKoef = 0.f;
    entt::entity planet = entt::null;
};

struct InteractableWithPlanet
{
    entt::entity planet = entt::null;
};

struct Bullet
{
    float lifetimeS = 0.f;
};

struct Camera
{
    entt::entity target = entt::null;
    sf::Vector2f size = {};
    float offsetFromThePlanet = 0.f;
};

struct Asteroid
{
};
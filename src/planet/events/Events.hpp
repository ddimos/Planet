#pragma once
#include <SFML/System/Vector2.hpp>

struct PlayerShootBulletEvent
{
    PlayerShootBulletEvent(sf::Vector2f _position, sf::Vector2f _direction)
        : position(_position), direction(_direction) 
    {}

    sf::Vector2f position = {};
    sf::Vector2f direction = {};
};

struct CollisionEvent
{
    CollisionEvent(entt::entity _entityA, entt::entity _entityB)
        : entityA(_entityA), entityB(_entityB) 
    {}

    entt::entity entityA = entt::null;
    entt::entity entityB = entt::null;
};

struct PlayerLaunchMissileEvent
{
    PlayerLaunchMissileEvent(sf::Vector2f _position, sf::Vector2f _direction)
        : position(_position), direction(_direction) 
    {}

    sf::Vector2f position = {};
    sf::Vector2f direction = {};    // maybe don't need it
};

struct InteractionEvent
{
    enum class InteractionState
    {
        STARTED,
        FINISHED
    };

    InteractionEvent(entt::entity _entityA, entt::entity _entityB, InteractionState _state)
        : entityA(_entityA), entityB(_entityB)
        , state(_state) 
    {}

    entt::entity entityA = entt::null;
    entt::entity entityB = entt::null;
    InteractionState state = InteractionState::STARTED;
};

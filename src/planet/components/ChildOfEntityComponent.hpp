#pragma once

#include <SFML/System/Vector2.hpp>

struct ChildOfEntity
{
    entt::entity parent = entt::null;
    sf::Vector2f offset{};
};

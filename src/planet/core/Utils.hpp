#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

inline float toRad(float _angleInDeg)
{
    return _angleInDeg * M_PI / 180.f;
}

inline float toDeg(float _angleInRad)
{
    return _angleInRad * 180.f / M_PI;
}

inline float vectorLength(const sf::Vector2f& _v)
{
    return sqrtf(_v.x * _v.x + _v.y * _v.y);
}

inline float dotProduct(const sf::Vector2f& _v1, const sf::Vector2f& _v2)
{
    return _v1.x * _v2.x + _v1.y * _v2.y;
}

float angleBetweenVectors(const sf::Vector2f& _vec1, const sf::Vector2f& _vec2);
sf::Vector2f normalizedVector(const sf::Vector2f _v);

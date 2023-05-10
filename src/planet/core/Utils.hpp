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

inline sf::Vector2f tangentVector(const sf::Vector2f _v)
{
    return sf::Vector2f{-_v.y, _v.x};
}

float angleBetweenVectors(const sf::Vector2f& _vec1, const sf::Vector2f& _vec2);
sf::Vector2f normalizedVector(const sf::Vector2f _v);

// -------------------------------------------------

struct PolarPos
{
    float phi = 0.f;
    float r = 0.f;
};

inline PolarPos convertToPolar(sf::Vector2f _pos)
{
    return PolarPos{ .phi = toDeg(atan2f(_pos.y, _pos.x) - M_PI_2), .r = sqrtf(_pos.x*_pos.x + _pos.y*_pos.y) };
}

inline sf::Vector2f convertToCartesian(PolarPos _polar)
{
    return sf::Vector2f{ -1.f * _polar.r * sinf(toRad(_polar.phi)), _polar.r * cosf(toRad(_polar.phi)) };;
}
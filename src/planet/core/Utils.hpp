#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

constexpr float EPSILON = 0.0001f; 

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
sf::Vector2f normalizedVector(sf::Vector2f _v);

sf::Vector2f rotateVector(sf::Vector2f _v, float _angle);
sf::Vector2f rotateVectorInverse(sf::Vector2f _v, float _angle);

// -------------------------------------------------

struct PolarPos
{
    float phi = 0.f;
    float r = 0.f;
};

inline PolarPos convertToPolar(sf::Vector2f _pos)
{
    return PolarPos{ .phi = toDeg(std::atan2(_pos.y, _pos.x)), .r = std::sqrt(_pos.x*_pos.x + _pos.y*_pos.y) };
}

inline sf::Vector2f convertToCartesian(PolarPos _polar)
{
    return sf::Vector2f{ _polar.r * std::cos(toRad(_polar.phi)), _polar.r * std::sin(toRad(_polar.phi)) };;
}

// -------------------------------------------------

inline bool approximatelyEqual(float _a, float _b)
{
    return fabs(_a - _b) <= std::max(fabs(_a), fabs(_b)) * EPSILON;
}

inline bool approximatelyEqual(sf::Vector2f _a, sf::Vector2f _b)
{
    return approximatelyEqual(_a.x, _b.x) && approximatelyEqual(_a.y, _b.y);
}

// -------------------------------------------------

inline float normalizeAngle(float _angle)
{
    return _angle + ((_angle > 180.f) ? -360.f : (_angle < -180.f) ? 360.f : 0.f);
}

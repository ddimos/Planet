#include "core/Utils.hpp"

float angleBetweenVectors(const sf::Vector2f& _vec1, const sf::Vector2f& _vec2)
{
    float angle = atan2f(_vec2.y, _vec2.x) - atan2(_vec1.y, _vec1.x);
    
    if (angle > M_PI)        { angle -= 2 * M_PI; }
    else if (angle <= -M_PI) { angle += 2 * M_PI; }

    return angle;
}

sf::Vector2f normalizedVector(const sf::Vector2f _v)
{
    sf::Vector2f vOut{_v};
    const float len = vectorLength(_v);
    if(len > 0.f)
    {   
        float invLen = 1.0f / len;
        vOut = invLen * _v;
    }
    return vOut;
}
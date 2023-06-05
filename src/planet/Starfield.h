#pragma once
#include <SFML/Graphics.hpp>
#include "core/Random.h"

class Starfield 
{
public:
    Starfield(sf::RenderWindow& _windowRef);

    void init();
    void update(sf::Vector2f _cameraPos);

private:
    class StarfieldLayer : public sf::Drawable, public sf::Transformable
    {
    public:
        StarfieldLayer(Random& _randomizer, unsigned _numberOfStars, sf::Vector2f _positionRange, sf::Vector2f _sizeRange, sf::Vector2i _alphaRange, float _offset);

        float getOffset() const { return m_offset; }

    private:
        void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;
    
        sf::VertexArray m_vertices;
        float m_offset = 0.0f;
    };

    sf::RenderWindow& m_windowRef;
    Random m_randomizer;

    std::vector<StarfieldLayer> m_stars;
    sf::Vector2f m_prevCameraPos{};
};

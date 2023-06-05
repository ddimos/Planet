#include "Starfield.h"

namespace
{
    // http://www.vendian.org/mncharity/dir3/starcolor/
    const std::vector<sf::Color> starColors{
        sf::Color{155, 176, 255},   // O
        sf::Color{170, 191, 255},   // B
        sf::Color{202, 215, 255},   // A
        sf::Color{248, 247, 255},   // F
        sf::Color{255, 244, 234},   // G
        sf::Color{255, 210, 161},   // K
        sf::Color{255, 204, 111}    // M
    };

    constexpr int NUMBER_OF_STARS = 7500;
} // namespace

Starfield::Starfield(sf::RenderWindow& _windowRef)
    : m_windowRef(_windowRef)
{
}

void Starfield::init()
{
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.03f, {-4000.f, 4000.f}, {5.f, 10.f}, {240, 255}, 0.5f ));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.02f, {-3000.f, 3000.f}, {4.f, 8.f},  {210, 255}, 0.6f ));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.03f, {-2000.f, 2000.f}, {3.f, 7.f},  {200, 245}, 0.65f));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.04f, {-2000.f, 2000.f}, {2.f, 6.f},  {160, 230}, 0.7f ));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.06f, {-2000.f, 2000.f}, {2.f, 5.f},  {150, 220}, 0.75f));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.07f, {-2000.f, 2000.f}, {2.f, 4.f},  {140, 210}, 0.8f ));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.08f, {-2000.f, 2000.f}, {2.f, 3.f},  {130, 200}, 0.85f));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.11f, {-2000.f, 2000.f}, {2.f, 3.f},  {120, 190}, 0.9f ));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.23f, {-2000.f, 2000.f}, {2.f, 3.f},  {100, 180}, 0.95f));
    m_stars.emplace_back(StarfieldLayer(m_randomizer, NUMBER_OF_STARS * 0.33f, {-2000.f, 2000.f}, {2.f, 3.f},  {80 , 160}, 1.0f ));
}

void Starfield::update(sf::Vector2f _cameraPos)
{
    sf::Vector2f offset = m_prevCameraPos - _cameraPos;
    m_prevCameraPos = _cameraPos;

    for(auto& layer : m_stars)
    {
        layer.move(-1.f * offset * layer.getOffset());
        m_windowRef.draw(layer);
    }
}

Starfield::StarfieldLayer::StarfieldLayer(Random& _randomizer, unsigned _numberOfStars, sf::Vector2f _positionRange, sf::Vector2f _sizeRange, sf::Vector2i _alphaRange, float _offset)
    : m_vertices(sf::Quads, 4 * _numberOfStars)
    , m_offset(_offset)
{
    for (int i = 0; i < 4 * _numberOfStars; i += 4)
    {
        int colorIndex = _randomizer.rand(0, starColors.size());
        sf::Color color{ starColors[colorIndex] };
        color.a = _randomizer.rand(_alphaRange.x, _alphaRange.y);
        
        float size = _randomizer.rand(10.f * _sizeRange.x, 10.f * _sizeRange.y) / 10.f;
        auto position = sf::Vector2f(
            _randomizer.rand(_positionRange.x, _positionRange.y),
            _randomizer.rand(_positionRange.x, _positionRange.y));
            
        m_vertices[ i ].position = position;
        m_vertices[ i ].color    = color;
        m_vertices[i+1].position = { position.x + size, position.y };
        m_vertices[i+1].color    = color;
        m_vertices[i+2].position = { position.x + size, position.y + size };
        m_vertices[i+2].color    = color;
        m_vertices[i+3].position = { position.x,        position.y + size };
        m_vertices[i+3].color    = color;
    }
}

void Starfield::StarfieldLayer::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
    _states.transform *= getTransform();
    _states.texture = nullptr;
    _target.draw(m_vertices, _states);
}

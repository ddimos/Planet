#include "core/Game.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "core/Engine.hpp"

Game::Game(Engine& _engineRef)
    : m_engineRef(_engineRef)
    , m_window(_engineRef.getWindow())
    , m_systemManager(_engineRef)
{
}

void Game::init()
{
    m_systemManager.init();
}

void Game::update()
{
    handleEvents();

}
void Game::fixedUpdate(float _dt)
{
    m_systemManager.update(_dt);
}

void Game::render()
{
    m_systemManager.render();
    sf::CircleShape shape(100.f);
    shape.setPosition(sf::Vector2f(412.f, 284.f));
    shape.setFillColor(sf::Color::Blue);
    m_window.draw(shape);
}

void Game::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_engineRef.stop();
    }
}
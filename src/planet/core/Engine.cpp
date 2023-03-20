#include "core/Engine.hpp"

constexpr unsigned UPDATES_PER_SEC = 60;
constexpr float DT = 1.f / UPDATES_PER_SEC;

Engine::Engine()
    : m_game(*this)
{
	m_window.create(sf::VideoMode(1024, 768), "Planet");
	m_window.setFramerateLimit(UPDATES_PER_SEC);
}

void Engine::stop()
{
	m_isRunning = false;
}

void Engine::start()
{
    m_game.init();
    
    sf::Clock clock;
    float accumulator = 0.f;

    while (m_isRunning)
    {
        sf::Time elapsed = clock.restart();
        accumulator += elapsed.asSeconds();

		m_game.update();
        while (accumulator >= DT)
        {
		    m_game.fixedUpdate(DT);
            accumulator -= DT;
        }

        m_window.clear();
    
		m_game.render();

        m_window.display();
    }
}
#include "systems/AsteroidSystem.hpp"

#include "components/Components.hpp"
#include "core/Engine.hpp"
#include "core/ResourceManager.hpp"
#include "core/Utils.hpp"
#include "core/Log.hpp"
#include "core/Random.h"
#include <SFML/Window/Keyboard.hpp>

AsteroidSystem::AsteroidSystem()
{
    m_randomizer.init(time(NULL));
}

void AsteroidSystem::onUpdate(float _dt)
{
    // m_spawnTimeS -= _dt;
    // if (m_spawnTimeS > 0.f)
    //     return;
    // m_spawnTimeS = m_spawnFrequencyS;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        if (!m_isPressed)
        {
            m_isPressed = true;
            spawn();
        }
    }
    else
    {
        m_isPressed = false;
    }
}

void AsteroidSystem::spawn()
{    
    auto asteroid = m_registryRef->create();

    m_registryRef->emplace<Asteroid>(asteroid);
    auto& body = m_registryRef->emplace<Body>(asteroid);
    body.velocity = {(float)m_randomizer.rand(-50, 50), (float)m_randomizer.rand(-50, 50)};
    auto& transform = m_registryRef->emplace<Transform>(asteroid);
    transform.position = {(float)m_randomizer.rand(0, 700), (float)m_randomizer.rand(0, 700)};
    auto& renderable = m_registryRef->emplace<Renderable>(asteroid);
    renderable.sprite.setTexture(m_engineRef->getResourceManager().getTexture("asteroid"));
    renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
    renderable.sprite.setScale(sf::Vector2f{0.2f, 0.2f});
    auto& collidable = m_registryRef->emplace<Collidable>(asteroid);
    collidable.radius = renderable.sprite.getGlobalBounds().height / 2.f;
    auto& interactableWithPlanet = m_registryRef->emplace<InteractableWithPlanet>(asteroid);
    interactableWithPlanet.planet = m_registryRef->ctx().get<entt::entity>();
    interactableWithPlanet.gravityKoef = 5000000.f;

    LOG("Spawn " + tstr((unsigned)asteroid));
}

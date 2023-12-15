#include "systems/AsteroidSystem.hpp"

#include "components/Components.hpp"
#include "core/Engine.hpp"
#include "core/ResourceManager.hpp"
#include "core/Utils.hpp"
#include "core/Log.hpp"
#include "core/Random.h"

#include "EntityType.hpp"

#include <SFML/Window/Keyboard.hpp>

AsteroidSystem::AsteroidSystem()
    : m_maxDistanceFromPlanet{2500.f}
    , m_minDistanceFromPlanet{1500.f}
    , m_maxSpawnFrequencyS{10}
    , m_minSpawnFrequencyS{4}
    , m_spawnTimeS{m_minSpawnFrequencyS}
{
    m_randomizer.init(time(NULL));

    m_spawnTimeS = getNewSpawnFrequency();
}

void AsteroidSystem::onUpdate(float _dt)
{
    checkSpawn(_dt);
}

void AsteroidSystem::checkSpawn(float _dt)
{
    m_spawnTimeS -= _dt;
    if (m_spawnTimeS <= 0.f)
    {
        m_spawnTimeS = getNewSpawnFrequency();
        spawn();
    }

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

float AsteroidSystem::getNewSpawnFrequency()
{
    return m_randomizer.rand(m_minSpawnFrequencyS, m_maxSpawnFrequencyS);
}

void AsteroidSystem::spawn()
{
    using namespace entt::literals;

    entt::entity planet = m_registryRef->ctx().get<entt::entity>("planet"_hs);

    PolarPos polar;
    polar.r = m_randomizer.rand(m_minDistanceFromPlanet, m_maxDistanceFromPlanet);
    polar.phi = m_randomizer.rand(0, 360);
    const float speed = m_randomizer.rand(20, 50);
    const float scale = m_randomizer.rand(10, 50) / 100.f;
    const float rotation = m_randomizer.rand(0, 360);

    auto& planetTrans = m_registryRef->get<Transform>(planet);
    sf::Vector2f asteroidPosPlanet = convertToCartesian(polar); // In the planet frame of reference 
    sf::Vector2f asteroidPos = planetTrans.position + asteroidPosPlanet;
    auto direction = normalizedVector(-asteroidPosPlanet);

    auto asteroid = m_registryRef->create();

    auto& entityComp = m_registryRef->emplace<EntityComponent>(asteroid);
    entityComp.type = EntityType::ASTEROID;

    m_registryRef->emplace<Asteroid>(asteroid);

    m_registryRef->emplace<Damageable>(asteroid);
    
    auto& body = m_registryRef->emplace<Body>(asteroid);
    body.velocity = speed * direction;

    auto& transform = m_registryRef->emplace<Transform>(asteroid);
    transform.position = asteroidPos;
    transform.rotation = rotation;
    transform.scale = scale;

    auto& renderable = m_registryRef->emplace<Renderable>(asteroid);
    renderable.sprite.setTexture(m_engineRef->getResourceManager().getTexture("asteroid"));
    renderable.sprite.setOrigin(renderable.sprite.getLocalBounds().width / 2.f, renderable.sprite.getLocalBounds().height / 2.f);
    renderable.sprite.setScale(sf::Vector2f{scale, scale});

    auto& collidable = m_registryRef->emplace<Collidable>(asteroid);
    collidable.radius = renderable.sprite.getGlobalBounds().height / 2.f;
    collidable.typeFlag = entityComp.type;
    collidable.canColideWithFlags = EntityType::PLAYER | EntityType::PROJECTILE | EntityType::ASTEROID | EntityType::PLANET;
    
    auto& gravity = m_registryRef->emplace<Gravity>(asteroid);
    gravity.planet = m_registryRef->ctx().get<entt::entity>("planet"_hs);
    gravity.gravityKoef = 5000000.f;

    auto& uiMap = m_registryRef->emplace<UIMapComponent>(asteroid);
    uiMap.color = sf::Color::Red;
    uiMap.radius = renderable.sprite.getGlobalBounds().height / 2.f;

    LOG_DEBUG("Spawn " + tstr((unsigned)asteroid));
}

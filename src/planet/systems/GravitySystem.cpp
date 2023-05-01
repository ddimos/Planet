#include "systems/GravitySystem.hpp"
#include "components/BodyComponent.hpp"
#include "components/GravityComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Utils.hpp"

void GravitySystem::onUpdate(float _dt)
{
    for(auto &&[entity, transform, body, gravity] : m_registryRef->view<Transform, Body, Gravity>().each())
    {
        if (!m_registryRef->valid(gravity.planet))
            continue;

        auto& planet = m_registryRef->get<Transform>(gravity.planet);
        sf::Vector2f vecFromEntityToPlanet = planet.position - transform.position;
        float r = vectorLength(vecFromEntityToPlanet);
        body.force += gravity.gravityKoef / (r*r) * normalizedVector(vecFromEntityToPlanet); 
    }
}

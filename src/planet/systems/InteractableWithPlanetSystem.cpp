#include "systems/InteractableWithPlanetSystem.hpp"
#include "components/Components.hpp"
#include "core/Utils.hpp"

namespace
{
    float adjustRotation(sf::Vector2f _planetPos, const Transform& _entityTransform, float _rotThreshold, float _rotSpeed, float _dt)
    {
        auto normalOfEntity =  sf::Vector2f{-sinf(toRad(_entityTransform.rotation)), cosf(toRad(_entityTransform.rotation))};
        auto vecFromEntityToPlanet =  _planetPos - _entityTransform.position;

        float angle = toDeg(angleBetweenVectors(normalOfEntity, vecFromEntityToPlanet));
        if (fabsf(angle) > _rotThreshold)
        {
            float speed = _rotSpeed * ((angle > 0.f) ? 1.f : -1.f);
            return speed * _dt;
        }

        return 0.0f;
    }
} // namespace


void InteractableWithPlanetSystem::onUpdate(float _dt)
{
    for(auto &&[entity, transform, body, interactableWithPlanet] : m_registryRef->view<Transform, Body, InteractableWithPlanet>().each())
    {
        auto& planet = m_registryRef->get<Transform>(interactableWithPlanet.planet);
        transform.rotation += adjustRotation(planet.position, transform, m_rotThreshold, m_rotSpeed, _dt);
        sf::Vector2f vecFromEntityToPlanet = planet.position - transform.position;
        float r = vectorLength(vecFromEntityToPlanet);
        body.force += interactableWithPlanet.gravityKoef / (r*r) * normalizedVector(vecFromEntityToPlanet); 
    }
}

#include "systems/InteractableWithPlanetSystem.hpp"
#include "components/InteractableWithPlanetComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Utils.hpp"

namespace
{
    float adjustRotation(sf::Vector2f _planetPos, const Transform& _entityTransform)
    {
        auto vecFromEntityToPlanet =  _planetPos - _entityTransform.position;
        auto normal = sf::Vector2f{0.0f, 1.f};

        float angle = toDeg(angleBetweenVectors(normal, vecFromEntityToPlanet));
        return angle;
    }
} // namespace


void InteractableWithPlanetSystem::onUpdate(float _dt)
{
    for(auto &&[entity, transform, interactableWithPlanet] : m_registryRef->view<Transform, InteractableWithPlanet>().each())
    {
        auto& planet = m_registryRef->get<Transform>(interactableWithPlanet.planet);
        transform.rotation = adjustRotation(planet.position, transform);
        sf::Vector2f vecFromEntityToPlanet = planet.position - transform.position;
    }
}

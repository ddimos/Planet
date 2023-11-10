#include "systems/TargetingSystem.hpp"
#include "events/Events.hpp"
#include "components/Components.hpp"
#include "core/Engine.hpp"
#include "core/Utils.hpp"
#include "EntityType.hpp"


void TargetingSystem::onUpdate(float _dt)
{
    for (auto &&[missileEntity, transformComp, bodyComp, shootableComp] : m_registryRef->view<Transform, Body, Shootable>().each())
    {
        if (!shootableComp.canHaveTarget)
            continue;

        if (!m_registryRef->valid(shootableComp.target))
        {
            shootableComp.target = defineTarget(transformComp.position);
            if (!m_registryRef->valid(shootableComp.target))
                continue;
        }

        // TODO make a better missile turning
        auto& targetTransform = m_registryRef->get<Transform>(shootableComp.target);
        sf::Vector2f direction = normalizedVector(targetTransform.position - transformComp.position);
        float angle = angleBetweenVectors(bodyComp.velocity, direction);
        bodyComp.velocity = shootableComp.speed * rotateVector(normalizedVector(bodyComp.velocity), (angle > 0.f) ? 1.f : -1.f);
        transformComp.rotation = -toDeg(angleBetweenVectors(-direction, sf::Vector2f(-1.f, 0.f)));
    }
}

entt::entity TargetingSystem::defineTarget(sf::Vector2f _position)
{
    // TODO Targetable
    float minDistance = -1;
    entt::entity theClosestTarget = entt::null;

    for (auto &&[targetEntity, transformComp] : m_registryRef->view<Transform, Damageable>().each())
    {
        sf::Vector2f direction = transformComp.position - _position; 
        float distance = vectorLength(direction);
        if (minDistance <= -1 || distance < minDistance)
        {
            minDistance = distance;
            theClosestTarget = targetEntity;
        }
    }

    return theClosestTarget;
}

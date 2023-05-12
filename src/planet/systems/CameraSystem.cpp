#include "systems/CameraSystem.hpp"
#include "components/CameraComponent.hpp"
#include "components/InteractableWithPlanetComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/TransformComponent.hpp"

#include "core/Utils.hpp"

void CameraSystem::onUpdate(float _dt)
{
    auto view = m_registryRef->view<Transform, Camera>();
    for(auto entity : view)
    {
        auto& cameraTransform = view.get<Transform>(entity);
        auto& camera = view.get<Camera>(entity);

        if (m_registryRef->valid(camera.target) && m_registryRef->all_of<Transform>(camera.target))
        {        
            auto& targetTransform = m_registryRef->get<Transform>(camera.target);            
            sf::Vector2f targetedCameraPosition{ targetTransform.position };

            if (camera.offsetFromThePlanet != 0.f && m_registryRef->all_of<InteractableWithPlanet, Player>(camera.target))
            {
                auto& targetInterWithPlanet = m_registryRef->get<InteractableWithPlanet>(camera.target);
                auto& planetTransform = m_registryRef->get<Transform>(targetInterWithPlanet.planet);
                
                sf::Vector2f direction = normalizedVector(targetTransform.position - planetTransform.position);
                targetedCameraPosition = targetTransform.position + direction * camera.offsetFromThePlanet;  
            }
            if (!approximatelyEqual(cameraTransform.position, targetedCameraPosition))
                cameraTransform.position = (targetedCameraPosition - cameraTransform.position) * camera.speed * _dt + cameraTransform.position;
            
            if (!approximatelyEqual(cameraTransform.rotation, targetTransform.rotation))
            {
                float angleDif = normalizeAngle(targetTransform.rotation - cameraTransform.rotation);
                cameraTransform.rotation = normalizeAngle((angleDif) * camera.angularSpeed * _dt + cameraTransform.rotation);
            }
        }
        // TODO else case
    }
}
